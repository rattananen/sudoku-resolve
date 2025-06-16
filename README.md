## Usages

### Interactive mode

```bash
sudoku_resolve --interactive
```

### Input as text file

```bash
sudoku_resolve test_in.txt
```

### Input/output as text file

```bash
sudoku_resolve test_in.txt output.txt
```
## Usages for input as iamge (OCR)

### Examaple 1

```bash
sudoku_ocr test.png
```
 - This command will looking for `eng.traineddata` file (tesseract trained models) in current folder if you don't have `TESSDATA_PREFIX` (path to Tesseract trained models folder) in enviroment vaiable.
 - You can download trained models from https://github.com/tesseract-ocr/tessdata_best or what you prefer. 

 
### Examaple 2

```bash
sudoku_ocr test.png /path/to/tesseract-trained-models-folder
```
- You can specific Tesseract trained models folder by this one.
