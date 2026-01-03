# PNG to C Array Bitmap

## How to use..

This may eventually become a published PyPI package, but for now you can just
run the script directly.

### Activate Python Virtual Environment and Install Dependencies

```
cd mcu-fun/scripts/png2carray
. venv/bin/activate
pip install -r requirements.txt
```

### Run the Script
```
python png2carray/png2c.py -o bitmaps.h sample1.png
```

Your C code will be in the `bitmaps.h` file.
