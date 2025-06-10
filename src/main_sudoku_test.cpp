#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <iostream>
#include <memory>

int main()
{
    using Tess_str = std::unique_ptr<char[]>;

    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(nullptr, "eng")) {
        std::cerr << "Could not initialize tesseract.\n";
        return 1;
    }

    // Open input image with leptonica library
    Pix* image = pixRead("E:\\works\\sudoku-resolve\\assets\\test_in.png");
    api->SetVariable("tessedit_char_whitelist", "0123456789");
    api->SetImage(image);
    // Get OCR result
   Tess_str text(api->GetUTF8Text());

   std::cout << "OCR output:\n" << text.get();


    // Destroy used object and release memory
    api->End();
    delete api;
    //delete[] outText;
    pixDestroy(&image);

    return 0;
}