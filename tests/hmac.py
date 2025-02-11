import hashlib

def hmac_sha256(key, message):
    block_size = 64  # Tamaño de bloque de SHA-256 (en bytes)
    hash_size = 32   # Tamaño del hash de SHA-256 (en bytes)

    print("Key:", key.hex())
    # Paso 1: Preparar la clave
    if len(key) > block_size:
        key = hashlib.sha256(key).digest()  # Si la clave es > 64 bytes, se hashea
    if len(key) < block_size:
        key += b'\x00' * (block_size - len(key))  # Rellena con ceros si es < 64 bytes

    print("Key:", key.hex())
    # Paso 2: Calcular ipad y opad
    ipad = bytes([0x36] * block_size)
    opad = bytes([0x5c] * block_size)

    # Paso 3: Calcular el valor interno
    inner_key = bytes([k ^ i for k, i in zip(key, ipad)])  # XOR entre key e ipad
    inner_message = inner_key + message  # Concatenar con el mensaje
    inner_hash = hashlib.sha256(inner_message).digest()  # Aplicar SHA-256

    # Paso 4: Calcular el valor externo
    outer_key = bytes([k ^ o for k, o in zip(key, opad)])  # XOR entre key y opad
    outer_message = outer_key + inner_hash  # Concatenar con el valor interno
    hmac_result = hashlib.sha256(outer_message).digest()  # Aplicar SHA-256

    return hmac_result

# Ejemplo de uso
key = b"mi_clave_secreta"
message = b"mensaje_para_autenticar"
hmac = hmac_sha256(key, message)
print("HMAC-SHA256:", hmac.hex())