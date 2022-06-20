# Aes_Viewer

Implementation en c++ de l'algorithme de chiffrement [AES](https://fr.wikipedia.org/wiki/Advanced_Encryption_Standard)

[AES](https://fr.wikipedia.org/wiki/Advanced_Encryption_Standard):
est un algorithme de chiffrement symetrique par blocs de 128 bytes

## Install
- `git clone https://github.com/Bibi210/Aes_Viewer.git`

- `cd Aes_Viewer`

- `make`

- ``` ./Aes_img Images/blue_sky.jpeg ```

## Commande du viewer 
Clique Droit -> Ouverture du menu

### Menu :

Encrypt ECB -> Chiffrement via le mode
[Electronic Code Book](https://fr.wikipedia.org/wiki/Mode_d%27op%C3%A9ration_(cryptographie)#Dictionnaire_de_codes_:_%C2%AB_Electronic_codebook_%C2%BB_(ECB) "Electronic Code Book").\
Encrypt CBC -> Chiffrement via le mode [Cipher Block Chaining](https://fr.wikipedia.org/wiki/Mode_d%27op%C3%A9ration_(cryptographie)#Encha%C3%AEnement_des_blocs_:_%C2%AB_Cipher_Block_Chaining_%C2%BB_(CBC) "Cipher Block Chaining").\
Decrypt -> Dechiffrement de l'image

## Explications

Ce Projet se decoupe en deux module AES et Img_Viewer\
Le Code source de AES :
```Lib/aes.hpp et Lib/aes.cpp```\
Le Code de l'image Viewer :
```./Aes_img.cpp + Lib/stb_image.h``` 

## Viewer

Basée sur le Menu utilisé en cours de Algorithmique Avancé.\
Utilisation de la library [STB](https://github.com/nothings/stb) pour la lecture d'image
 

## AES
Documments : [AES_SYNT ](https://www.emse.fr/~dutertre/documents/synth_AES128.pdf),[ Reference Doc](https://nvlpubs.nist.gov/nistpubs/fips/nist.fips.197.pdf)\
Block Size = 128 bits\
Key Size = 128 bits

## Todo List
### AES 128 Functions :

Enc_Dec :
- [X] KeyExpension
- [X] AddRoundKey
  
Encrypt :
- [X] Sub_Block
- [X] MixColumns
- [X] ShiftRows
  
Decrypt :  
- [X] UnSub_Block
- [X] UnMixColumns
- [X] UnShiftRows

Dibassi Brahima\
L3 Informatique
