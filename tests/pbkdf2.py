import hashlib
import hmac
import os

def pbkdf2_hmac_sha256(password, salt, iterations, key_length):
    # Convierte la contraseña y el salt a bytes si no lo están
    if isinstance(password, str):
        password = password.encode('utf-8')
    if isinstance(salt, str):
        salt = salt.encode('utf-8')

    # Tamaño del hash en bytes (SHA-256 produce 32 bytes)
    hash_size = 32

    # Número de bloques necesarios
    blocks_needed = (key_length + hash_size - 1) // hash_size

    # Inicializa la clave derivada
    derived_key = b''

    for i in range(1, blocks_needed + 1):
        # Calcula U_1 = HMAC-SHA256(Password, Salt || INT_32_BE(i))
        block = hmac.new(password, salt + int_to_bytes(i), hashlib.sha256).digest()
        
        u_prev = block

        # Calcula U_2 a U_c
        for _ in range(1, iterations):
            u_current = hmac.new(password, u_prev, hashlib.sha256).digest()            
            block = bytes([b1 ^ b2 for b1, b2 in zip(block, u_current)])
            u_prev = u_current

        # Concatena el bloque a la clave derivada
        derived_key += block
        

    # Trunca la clave derivada a la longitud deseada
    return derived_key[:key_length]

def int_to_bytes(i):
    # Convierte un entero a bytes en formato big-endian
    return i.to_bytes(4, 'big')

# Ejemplo de uso
password = "my_password"
#salt = os.urandom(16)  # Genera un salt aleatorio de 16 bytes
salt = "my_salto"
iterations = 10000
key_length = 8  # Longitud de la clave derivada en bytes (32 bytes = 256 bits)

derived_key = pbkdf2_hmac_sha256(password, salt, iterations, key_length)
print("Clave derivada:", derived_key.hex())