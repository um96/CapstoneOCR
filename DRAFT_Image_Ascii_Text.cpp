#include <iostream>
#include <fstream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc <= 2) {
		cout << "Syntax: " << argv[0] << " [Input Image] [Output File]" << endl;
		return 0;
	}

	char *outText;


	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();


	if (api->Init(NULL, "eng")) {
		fprintf(stderr, "Could not initialize tesseract ORC.\n");
		exit(1);

		return 0;
	}

	// Open input image with leptonica library
	Pix *image = pixRead(argv[1]);

	api->SetImage(image);
	// Get OCR result

	outText = api->GetUTF8Text();

	ofstream outFile;
	outFile.open(argv[2]);
	outFile << outText;
	outFile.close();

	// Print result of detection
	cout << outText;

	// Release variable & memory
	api->End();
	pixDestroy(&image);

	return 0;
}