from Crypto.Cipher import AES

# keyString = "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F".replace(" ", "")
keyString = "2B 7E 15 16 28 AE D2 A6 AB F7 15 88 09 CF 4F 3C".replace(" ", "")
key = bytearray.fromhex(keyString)
cipher = AES.new(key, AES.MODE_ECB)

plainfile = open("./data/plaintext.txt", 'r')
cipherfile = open("./data/ciphertext.txt", 'r')


plainLines = plainfile.readlines()
cipherLines = cipherfile.readlines()

errorSourceLines = []
errorManualEncryptLines = []
errorIdx = []
for idx, line in enumerate(plainLines):
    hexline = line.replace(" ", "")
    cipherLine = cipherLines[idx].replace(" ", "").lower()
    try:
        plain = bytearray.fromhex(hexline)
    except:
        print(idx)
    try:
        ciphertext = cipher.encrypt(plain)
        cipherString = bytes(ciphertext).hex()
        if cipherString.strip() != cipherLine.strip():
            errorIdx.append(idx+1)
            errorManualEncryptLines.append(cipherString)
            errorSourceLines.append(cipherLine)
    except:
        print(idx)
print("WRONG CIPHERS")
print(errorIdx)
        
plainfile.close()
cipherfile.close()

# with open("E:/FYP/data/10kRounds/plaintext.txt", 'r') as f:
#     with open("E:/FYP/data/10kRounds/ciphertest.txt", 'w') as g:
#         for plainLine in f.readlines():
#             print(plainLine)
#             hexline = plainLine.replace(" ", "")
#             plain = bytearray.fromhex(hexline)
#             ciphertext = cipher.encrypt(plain)
#             print(ciphertext)
#     g.close()
# f.close()