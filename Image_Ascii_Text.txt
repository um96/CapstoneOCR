#include <iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

using namespace std;

int main(char* argv[], int argc)
{
	if (argc <= 1) {
		cout << "Syntax: " << argv[1] << " [Input File]" << endl;
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

	// Print result of detection
	cout << outText << endl;
	

	// Release variable & memory
	api->End();
	delete[] outText;
	pixDestroy(&image);

	return 0;
}