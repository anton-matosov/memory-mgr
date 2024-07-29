
#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>


namespace po = boost::program_options;
namespace algo = boost::algorithm;

int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring path = L"e:\\projects\\memory-mgr\\trunk\\memory-mgr\\temp.i";
	std::wstring outPath;// = L"e:\\projects\\memory-mgr\\trunk\\memory-mgr\\temp.i.cpp";

	po::options_description descr;
	descr.add_options()
		( "path", po::wvalue(&path), "Path to the file to be processed" )
		( "out", po::wvalue(&outPath), "Path to the output file, default <path>.hpp" );

	po::parse_command_line( argc, argv, descr );

	if( outPath.empty() )
	{
		outPath = path + L".hpp";
	}

	std::wifstream inFile( path.c_str() );
	std::wofstream outFile( outPath.c_str() );

	std::wstring tmpLine;
	tmpLine.resize( 2048 );

	bool prevWasEmpty = false;
	while( inFile )
	{
		inFile.getline( (wchar_t*)tmpLine.c_str(), tmpLine.capacity() );

		std::wstring line = tmpLine.c_str();
		algo::trim( line );

		if( ! line.empty() && line.substr( 0, 5 ) != L"#line" )
		{
			if( prevWasEmpty )
			{
				outFile << L'\n';
				prevWasEmpty = false;
			}
			outFile << tmpLine.c_str() << L'\n';
		}

		if( ! prevWasEmpty && line.empty() )
		{
			prevWasEmpty = true;
		}
	}

}
