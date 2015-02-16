#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <map>
#include <cassert>

/*
 * Reads the given input file and popluates the testPath and links.
 * @param inputFile the input file to read
 * @param testPath the path to resolve
 * @param links the map of links and paths
 */
void readInput( const std::string& inputFile,
				std::string& testPath,
				std::map< std::string, std::string >& links )
{
	links.clear();

	// read the number of symbolic links
	std::ifstream in( inputFile.c_str(), std::ios::in );

	int numLinks = std::numeric_limits< int >::max();
	in >> numLinks;

	// read each symbolic link
	for ( int i = 0; i < numLinks; ++i )
	{
		std::string line, link, path;
		in >> line;

		// split the line into the link and path
		const int SPLIT = line.find( ':' );
		link = line.substr( 0, SPLIT );
		path = line.substr( SPLIT + 1, std::string::npos );

		if ( link.back() == '/' )
			link.pop_back();
		if ( path.back() == '/' )
			path.pop_back();

		links[ link ] = path;
	}

	// read the input path
	in >> testPath;

	in.close();
}

std::string resolveLinks( const std::string& testPath, std::map< std::string, std::string >& links )
{
	// for all substr in testPath, check for a path in the map
	std::string resolved = testPath;
	size_t i = 1;
	size_t split = std::string::npos;
	while ( i < resolved.length() )
	{
		split = resolved.find( '/', i );
		std::string link = resolved.substr( 0, split );
		
		if ( links.find( link ) != links.end() )
		{
			std::string path = links[ link ];
			resolved = path.append( resolved.substr( split, std::string::npos ) );
		}
		else
		{
			i += split;
		}
	}
	
	return resolved;
}

void myAssert( const std::string& input, 
			   const std::string& output,
			   const std::string& expected )
{
	std::cout << "Input    : " << input << std::endl
			  << "Output   : " << output << std::endl
			  << "Expected : " << expected << std::endl
			  << std::endl;

	assert( output == expected );
}

int main( int argc, char* argv[] )
{
	std::string testPath;
	std::map< std::string, std::string > links;

	readInput( "input1.txt", testPath, links );
	myAssert( testPath, resolveLinks( testPath, links ), "/media/mmcstick/docs/office" );

	readInput( "input2.txt", testPath, links );
	myAssert( testPath, resolveLinks( testPath, links ), "/etc/modprobe.d/config/" );

	readInput( "input3.txt", testPath, links );
	myAssert( testPath, resolveLinks( testPath, links ), "/var/log-2014/rc" );

	readInput( "input4.txt", testPath, links );
	myAssert( testPath, resolveLinks( testPath, links ), "/usr/local/include/SDL/stan" );

	return 0;
}