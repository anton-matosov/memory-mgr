import requests
import tarfile
from hashlib import sha1
from pathlib import Path

vendor_libs = [
  # ("https://master.dl.sourceforge.net/project/boost/boost/1.51.0/boost_1_51_0.tar.bz2", "SHA1", "52ef06895b97cc9981b8abf1997c375ca79f30c5"),
  ("https://master.dl.sourceforge.net/project/boost/boost/1.85.0/boost_1_85_0.tar.bz2", "SHA1", "ed58c632befe0d299b39f9e23de1fc20d03870d7"),
]
destination = Path.cwd() / "vendor" / "downloads"
destination.mkdir(parents=True, exist_ok=True)

def download(destination_file, url, hash_type, hash_value):
  # Verify hash
  if destination_file.exists():
    with open(destination_file, "rb") as f:
      data = f.read()

      if hash_type == "SHA1":
        hash = sha1(data).hexdigest()
      else:
        raise NotImplementedError(f"Hash type {hash_type} not implemented")

      if hash != hash_value:
        print(f"Hash mismatch for {url}. Expected {hash_value}, but actual is {hash}. Removing download {destination_file}")
        destination_file.unlink()

  if not destination_file.exists():
    response = requests.get(url)
    response.raise_for_status()
    data = response.content

    hash = sha1(data).hexdigest()
    assert hash == hash_value, f"Hash mismatch for {url} Expected {hash_value}, but actual is {hash}"
    with open(destination_file, "wb") as f:
      f.write(data)

for url, hash_type, hash_value in vendor_libs:
  destination_file = destination / Path(url).name
  download(destination_file, url, hash_type, hash_value)

  suffixes = destination_file.suffixes
  if suffixes[-2:] == [".tar", ".gz"]:
    mode = "r:gz"
  elif suffixes[-2:] == [".tar", ".bz2"]:
    mode = "r:bz2"
  else:
    raise NotImplementedError(f"Unsupported file type {suffixes}")

  with tarfile.open(destination_file, mode) as tar:
    tar.extractall(destination, filter='data')
