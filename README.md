# 🔒 LSB Image Steganography in C Programming Language

A command-line implementation of **LSB (Least Significant Bit) Image Steganography** written in **C**.

This project hides a secret file inside a **BMP image** by modifying the least significant bits of the image pixels. The encoded image looks visually identical to the original while securely carrying hidden information. The secret file can later be recovered using the decoder.

---

## 📌 Features

- ✅ Encode a secret file into a BMP image
- ✅ Decode the hidden file from the encoded image
- ✅ Magic String verification before decoding
- ✅ Supports multiple secret file extensions
  - `.txt`
  - `.csv`
  - `.py`
- ✅ Automatic image capacity checking
- ✅ Preserves the original image quality
- ✅ Written entirely in C using file handling and bit manipulation

---

## 🛠️ Technologies Used

- C Programming
- File Handling
- Bit Manipulation
- BMP Image Format
- LSB (Least Significant Bit) Algorithm

---

## 📂 Project Structure

```
LSB_Steganography/
│
├── main.c
├── encode.c
├── decode.c
├── encode.h
├── decode.h
├── common.h
├── types.h
│
├── scenery.bmp          # Input image
├── top_secret.txt         # Secret file
├── stego.bmp              # Encoded image(obtained after encoding)
├── decoded.txt            # Decoded output(obtained after decoding)
│
└── README.md
```
---

## 🔍 How LSB Steganography Works

Every pixel in a BMP image consists of three color components:

- Red
- Green
- Blue

Each component is stored in one byte (8 bits).

Instead of changing the entire byte, only the **Least Significant Bit (LSB)** is modified to store secret information.

Example:

```
Original Byte : 10110110
Secret Bit    :        1
Modified Byte : 10110111
```

Changing only the last bit produces almost no visible difference in the image.

---

## 🔐 Encoding Process

The encoder performs the following steps:

1. Open the source BMP image.
2. Open the secret file.
3. Check whether the image has enough capacity.
4. Copy the BMP header.
5. Encode the Magic String.
6. Encode the secret file extension size.
7. Encode the secret file extension.
8. Encode the secret file size.
9. Encode the secret file contents.
10. Copy the remaining image data.

---

## 🔓 Decoding Process

The decoder performs:

1. Open the encoded BMP image.
2. Verify the Magic String.
3. Decode the file extension size.
4. Decode the file extension.
5. Create the output file.
6. Decode the secret file size.
7. Decode the hidden data.
8. Recover the original secret file.

---

## 🚀 Compilation

Compile using GCC:

```bash
gcc main.c encode.c decode.c -o stego
```

---

## ▶️ Encoding

Syntax

```bash
./stego -e <input.bmp> <secret_file> <output.bmp>
```

Example

```bash
./stego -e scenery.bmp top_secret.txt stego.bmp
```

---

## ▶️ Decoding

Syntax

```bash
./stego -d <stego.bmp> <output_filename>
```

Example

```bash
./stego -d stego.bmp decoded
```

Output

```
decoded.txt
```

---

## 📖 Concepts Covered

- Bitwise Operations
- Binary File Handling
- Image Processing Basics
- BMP File Format
- File I/O in C
- Data Hiding Techniques
- Steganography

---

## 💡 Future Improvements

- Support PNG images
- Password-protected decoding
- Encrypt data before embedding
- GUI using Qt or GTK
- Support large files
- Randomized embedding positions
- Compression before encoding

---

## 👨‍💻 Author
**Adithyan Anilkumar**

---

LinkedIn:https://www.linkedin.com/in/adithyan-anilkumar-a23129329/
