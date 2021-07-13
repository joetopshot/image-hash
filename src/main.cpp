#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include "md5-a.h"
#include "md5.h"

extern "C" {
    #include <jpeglib.h>
}

#define MD5_SIZE		16

// function declare
int Compute_file_md5(const char *file_path, char *md5_str);

using namespace std;

std::string compute_md5_hash(const char* file_name) {
    const int buffer_lines = 16;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPARRAY pJpegBuffer;
    MD5 md5 = MD5();

    // Open file.
    FILE *infile = fopen(file_name, "rb");
    if (infile == NULL) {
        cerr << "Can't open file \"" << file_name  << "\"" << endl;
        exit(1);
    }

    // Initialize decompression.
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);
    pJpegBuffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, cinfo.output_width * cinfo.output_components, buffer_lines);

    // Read JPEG.
    while (cinfo.output_scanline < cinfo.output_height) {
        int read_lines = jpeg_read_scanlines(&cinfo, pJpegBuffer, buffer_lines);

        for (int y = 0; y < read_lines; y++) {
            md5.update(pJpegBuffer[y], cinfo.output_components * cinfo.output_width);
        }
    }

    // Clean-up.
    fclose(infile);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    // Compute MD5.
    md5.finalize();
//    cout << md5.hexdigest() << endl;
    return md5.hexdigest();
}

int main(int argc, char *argv[]) {

    std::string md5Value;
    std::string hashValue;

    const char* fileName1 = "data/LS7_3551.JPG";
    md5Value = compute_md5_hash(fileName1);
    cout << "md5 result is " << md5Value << endl;

	char md5_value1[MD5_SIZE*2];
    int errCode = Compute_file_md5(fileName1, md5_value1);
    if (0 != errCode) {
    	cerr << "couldn't open file" << endl;
    }
    cout << "new md5 result is " << md5_value1 << endl;


    const char* fileName2 = "data/LS7_3552.JPG";
    md5Value = compute_md5_hash(fileName2);
    cout << "md5 result is " << md5Value << endl;

	char md5_value2[MD5_SIZE];
    errCode = Compute_file_md5(fileName2, md5_value2);
    if (0 != errCode) {
    	cerr << "couldn't open file" << endl;
    }
    cout << "new md5 result is " << md5_value2 << endl;

    return 0;
}

/**
 * compute the value of a file
 * @param  file_path
 * @param  md5_str
 * @return 0: ok, -1: fail
 */
int Compute_file_md5(const char *file_path, char *md5_str)
{
	unsigned char md5_value[MD5_SIZE];
	MD5_CTX md5;

	std::ifstream inputFile(file_path, std::ios::binary);
	if (!inputFile) {
		perror("open error");
		return -1;
	}

	inputFile.seekg(0, std::ios::end);
	int fileSize = inputFile.tellg();
	inputFile.seekg(0, std::ios::beg);
	unsigned char data[fileSize];

	inputFile.read((char *)&data, fileSize);
	inputFile.close();

	// init md5
	MD5Init(&md5);

	MD5Update(&md5, data, fileSize);
	MD5Final(&md5, md5_value);

	// convert md5 value to md5 string
	for(int i = 0; i < MD5_SIZE; i++)
	{
		snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);
	}

	return 0;
}
