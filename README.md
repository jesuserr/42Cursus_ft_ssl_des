# ft_ssl_des

A C implementation of symmetric encoding and encryption algorithms, extending the capabilities of the [ft_ssl_md5](https://github.com/jesuserr/42Cursus_ft_ssl_md5) project by adding BASE64 encoding and DES (Data Encryption Standard) cipher suites, recreating the behavior of OpenSSL's cipher commands.

## 📋 Project Overview

This is the second ft_ssl project in the 42 school Encryption & Security path. It builds directly on top of the previous [ft_ssl_md5](https://github.com/jesuserr/42Cursus_ft_ssl_md5) project (MD5, SHA-224, SHA-256, SHA-384, SHA-512) and adds symmetric encoding/encryption: **BASE64** and **DES** in multiple block cipher modes of operation, with full OpenSSL compatibility.

## 🎯 Implemented Algorithms

### Encoding
- **BASE64** — Binary-to-text encoding (RFC 4648)

### Encryption — DES Block Cipher Modes
- **DES-ECB** — Electronic Codebook Mode
- **DES-CBC** — Cipher Block Chaining Mode (`des` is an alias for `des-cbc`)
- **DES-CFB** — Cipher Feedback Mode *(bonus)*
- **DES-OFB** — Output Feedback Mode *(bonus)*

> ⚠️ **Security Notice:** DES is considered **cryptographically broken and obsolete**. Standardised in 1977, its 56-bit key length is far too short for modern security requirements. DES has been officially withdrawn by NIST and replaced by **AES (Advanced Encryption Standard)**. This implementation exists purely for educational purposes within the 42 school curriculum. **Do not use DES to protect real data.**

### Key Derivation
- **PBKDF2** — Password-Based Key Derivation Function 2 (RFC 2898 / RFC 8018) backed by a custom SHA-256 HMAC

## 🔧 Command-Line Interface

```
./ft_ssl <command> [flags] [file]

Standard commands:
  Message Digest commands:
    md5  sha224  sha256  sha384  sha512

  Cipher commands:
    base64  des  des-ecb  des-cbc  des-cfb  des-ofb
```

### General Flags
| Flag | Description |
|------|-------------|
| `-h` | Print complete help for all commands and exit |

### BASE64 Flags
| Flag | Description |
|------|-------------|
| `-e` | Encode mode (default) |
| `-d` | Decode mode |
| `-i <file>` | Input file |
| `-o <file>` | Output file |

### DES Flags
| Flag | Description |
|------|-------------|
| `-e` | Encrypt mode (default) |
| `-d` | Decrypt mode |
| `-a` | Encode/decode I/O in BASE64 |
| `-k <hex>` | Key in hexadecimal |
| `-p <pass>` | Password in ASCII (triggers PBKDF2 key derivation) |
| `-s <hex>` | Salt in hexadecimal (used with `-p`) |
| `-v <hex>` | Initialization Vector in hexadecimal |
| `-i <file>` | Input file |
| `-o <file>` | Output file |

When neither `-k` nor `-p` is provided the program prompts for a password interactively (like OpenSSL).

### Flag Combination Behaviour (for DES encryption)

The following table summarises what happens for every relevant combination of `-k`, `-p`, and `-s` during **encryption**. Decryption mirrors this: if the ciphertext begins with the `Salted__` header the salt is extracted automatically; otherwise the user must supply the same key or password+salt that was used to encrypt.

| `-k` | `-p` / interactive | `-s` | Key source | Salt used | `Salted__` header in output |
|:----:|:-----------------:|:----:|------------|-----------|:---------------------------:|
| ✗ | ✗ (interactive) | ✗ | PBKDF2-HMAC-SHA256 from console password | Random (from `/dev/urandom`) | ✅ prepended |
| ✗ | ✅ | ✗ | PBKDF2-HMAC-SHA256 from `-p` password | Random (from `/dev/urandom`) | ✅ prepended |
| ✗ | ✅ | ✅ | PBKDF2-HMAC-SHA256 from `-p` password | User-provided (`-s`) | ✗ not prepended |
| ✅ | ✗ | ✗ | Direct hex key (`-k`), no derivation | None | ✗ not prepended |
| ✅ | ✗ | ✅ | Direct hex key (`-k`), no derivation | `-s` is **ignored** | ✗ not prepended |
| ✅ | ✅ | any | `-k` takes priority, `-p` is **ignored** | `-s` is **ignored** | ✗ not prepended |

> **Note:** When the `Salted__` header is not present in the ciphertext, decryption requires the exact same `-k` key or `-p`+`-s` combination that was used during encryption — no automatic recovery is possible.

## 🚀 Installation & Usage

### Cloning the Repository
```bash
git clone https://github.com/jesuserr/42Cursus_ft_ssl_des.git
cd 42Cursus_ft_ssl_des
```

### Compilation
```bash
make          # Compile the project
make clean    # Remove object files
make fclean   # Remove all build artifacts
make re       # Rebuild everything
make norm     # Check code style (42 norminette)
```

### BASE64 Examples
```bash
# Encode
echo toto | ./ft_ssl base64 -e
dG90bwo=

# Decode
echo dG90bwo= | ./ft_ssl base64 -d
toto

# Decode with whitespace in input
echo "d G9 0bwo =" | ./ft_ssl base64 -d
toto

# File I/O
./ft_ssl base64 -e -i plain.txt -o encoded.b64
./ft_ssl base64 -d -i encoded.b64 -o plain.txt
```

### DES Examples
```bash
# Encrypt with explicit key (ECB + BASE64)
echo "foo bar" | ./ft_ssl des-ecb -k f1729d96e6b4ba41 -a
P9ijOev51zOeyXwI+tiYkg==

# Encrypt / Decrypt round-trip (CBC + BASE64)
echo "one deep secret" | ./ft_ssl des-cbc -a -k 7713c750f854d029 -v 3a83686806d5efb8
c1lMvZzE5HcPpA5OB+Z/Tu7CLSEsLd0+
echo "c1lMvZzE5HcPpA5OB+Z/Tu7CLSEsLd0+" | ./ft_ssl des-cbc -d -a -k 7713c750f854d029 -v 3a83686806d5efb8 
one deep secret

# Password-based encryption (PBKDF2 derives key + random generated salt)
echo "secret message" | ./ft_ssl des-ecb -p "MySuperSecurePassword" -a
U2FsdGVkX1/Ex+HIFfe5GdYYVXBUr8x/9SxcoMfw5v4=
echo "secret message" | ./ft_ssl des-ecb -p "MySuperSecurePassword" -a
U2FsdGVkX18LYRqEu59MfPPHvkcDrYIEuD/q+KeXFZE=

# Password-based encryption (PBKDF2 derives key + user salt)
echo "secret message" | ./ft_ssl des-ecb -p "MySuperSecurePassword" -a -s 0123456789abcdef
E6GB9Y5SIYyxHMnnzQUvQA==

# Interactive password prompt (mirrors OpenSSL behavior)
echo "secret message" | ./ft_ssl des-ecb -a -s 0123456789abcdef
enter encryption password:MySuperSecurePassword
Verifying - enter encryption password:MySuperSecurePassword
E6GB9Y5SIYyxHMnnzQUvQA==
```

### Cross-compatibility with OpenSSL
```bash
# Encrypt with ft_ssl, decrypt with OpenSSL
echo "hello" | ./ft_ssl des-cbc -a -k 0102030405060708 -v 0807060504030201 | \
  openssl des-cbc -d -a -K 0102030405060708 -iv 0807060504030201 \
  -provider legacy -provider default

# Encrypt with OpenSSL, decrypt with ft_ssl
echo "hello" | openssl des-ecb -K 6162636461626364 -a -provider legacy -provider default | \
  ./ft_ssl des-ecb -d -a -k 6162636461626364
```

## 🏗️ Project Structure

```
.
├── Makefile
├── README.md                         # This file 
├── libft/                            # Custom C library
└── srcs/
    ├── encode/                       # BASE64 encoding/decoding
    ├── encrypt/                      # DES encryption/decryption 
    │   ├── des_ecb.c                 # DES-ECB mode
    │   ├── des_cbc.c                 # DES-CBC mode
    │   ├── des_cfb.c                 # DES-CFB mode (bonus)
    │   ├── des_ofb.c                 # DES-OFB mode (bonus)
    │   ├── encrypt_block_cipher.c    # Core DES Feistel network
    │   ├── encrypt_password.c        # Interactive password input
    │   ├── encrypt_pbkdf2.c          # PBKDF2 key derivation
    │   ├── encrypt_sha256.c          # Internal SHA-256 for HMAC
    │   ...
    ├── hash/                         # Inherited from ft_ssl_md5
    ├── incs/                         # Header files
    ├── main.c                        # Entry point    
    └── utils/                        # General utility functions
```

## 🔬 Technical Implementation

### BASE64
- Encodes every 3 bytes of input into 4 ASCII characters from the standard 64-character alphabet
- Handles `=` padding for inputs not divisible by 3
- Decoder ignores whitespace (newlines, spaces) for OpenSSL compatibility
- Output line wrapping at 64 characters (OpenSSL convention)

### DES Block Cipher
- 64-bit block size, 64-bit key (56 effective bits after parity stripping)
- 16-round Feistel network with initial/final permutation, expansion, S-boxes and P-box
- Key padding: short hex keys are zero-padded; longer keys are truncated
- Block padding: PKCS-style size-difference byte padding (same as OpenSSL)

### Block Cipher Modes
| Mode | Description |
|------|-------------|
| ECB | Each block encrypted independently |
| CBC | Each plaintext block XOR'd with previous ciphertext block before encryption |
| CFB | Ciphertext feedback — turns block cipher into a self-synchronizing stream cipher |
| OFB | Output feedback — turns block cipher into a synchronous stream cipher |

### PBKDF2 Key Derivation
When a password is supplied (flag `-p` or interactive prompt) OpenSSL's legacy PBKDF is reproduced:
1. A random 8-byte salt is generated by reading from `/dev/random`, the kernel's cryptographically secure random number generator (or taken from `-s` if provided)
2. PBKDF2-HMAC-SHA256 derives the key (and IV for CBC/CFB/OFB modes) from the password and salt
3. The magic header `Salted__` + salt is prepended to the ciphertext (OpenSSL convention)

## 📝 Notes on Code Style and Design

Some code writing decisions may look unusual or unnecessarily verbose at first glance. This is mainly due to the constraints imposed by the **42 Norminette**, the coding style enforced by 42 school projects. Among other rules, Norminette limits functions to a maximum of 25 lines, forbids `for` loops, restricts variable declarations to the top of functions, and prohibits certain operators and constructs. These constraints can lead to design choices that would otherwise not be made in idiomatic C.

Another key takeaway from this project is the importance of **structuring code with future expansion in mind**. This is the second stage of a three-part series — [ft_ssl_md5](https://github.com/jesuserr/42Cursus_ft_ssl_md5) → **ft_ssl_des** (this project) → ft_ssl_rsa — so architectural decisions made here intentionally leave room to accommodate asymmetric cryptography and other additions in the next stage without requiring a full rewrite.

## 📝 License

This project is part of the 42 School curriculum and is intended for educational purposes.
