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

std::string resolveLinks( const std::string& inputPath, const std::map< std::string, std::string >& links )
{
	std::string resolved = inputPath;		// copy the input path

	size_t i = 1;
	while ( i < resolved.length() )
	{
		size_t split = resolved.find( '/', i );
		std::string link = resolved.substr( 0, split );
		
		if ( links.find( link ) != links.end() )
		{	// link needs resolved
			std::string path = links.find( link )->second;
			resolved = path + resolved.substr( split, std::string::npos );
			i = path.length();				// update index to match new path length
		}
		else
		{	// link was not found in the map
			i = link.length() + 1;			// update index to go to next token
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
	std::string inputPath;									// the path to resolve
	std::map< std::string, std::string > links;				// std::map< symlink, path >

	readInput( "input1.txt", inputPath, links );
	myAssert( inputPath, resolveLinks( inputPath, links ), "/media/mmcstick/docs/office" );
	
	// infinite loop
	//readInput( "input2.txt", testPath, links );
	//myAssert( testPath, resolveLinks( testPath, links ), "/etc/modprobe.d/config/" );

	readInput( "input3.txt", inputPath, links );
	myAssert( inputPath, resolveLinks( inputPath, links ), "/var/log-2014/rc" );

	readInput( "input4.txt", inputPath, links );
	myAssert( inputPath, resolveLinks( inputPath, links ), "/usr/local/include/SDL/stan" );

	readInput( "input4_short.txt", inputPath, links );
	myAssert( inputPath, resolveLinks( inputPath, links ), "/boone/SDL/stan" );

	return 0;
}