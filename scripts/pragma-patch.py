#!/usr/bin/env python

import re
import os
from pathlib import Path

# #ifndef MEMORY_MGR_TYPE_INFO_HPP_INCLUDED
# #define MEMORY_MGR_TYPE_INFO_HPP_INCLUDED

# // MS compatible compilers support #pragma once

# #if defined(_MSC_VER) && (_MSC_VER >= 1020)
# # pragma once
# #endif

include_guard = re.compile(r'.*HPP_INCLUDED\n')
enclosed_pragma_once = re.compile(r'\n// MS compatible.*\n\n#if.*_MSC_VER.*\n# pragma once\n#endif')
# enclosed_pragma_once = re.compile(r'#if.*_MSC_VER.*\n# pragma once\n#endif')

def patch_file(file_path):
  with open(file_path, 'r') as f:
    data = f.read()
  if enclosed_pragma_once.search(data):
    data = enclosed_pragma_once.sub('#pragma once', data)
    data = include_guard.sub('', data)

    with open(file_path, 'w') as f:
      f.write(data)

patch_file(Path('/Users/antonmatosov/Develop/memory-mgr/code/memory-mgr/include/memory-mgr/detail/sp_typeinfo.hpp'))
