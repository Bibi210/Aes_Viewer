#ifndef AES_HPP
#define AES_HPP

#include <array>
#include <bitset>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>
using namespace std;

// Source : https://en.wikipedia.org/wiki/Rijndael_S-box
#define ROTL8(x, shift) ((uint8_t)((x) << (shift)) | ((x) >> (8 - (shift))))

class Aes {
  //* Block to Cipher = 128 bits = 16 octets = 16 * sizeof(uint8_t)
  //* key =  128 bits = 16 octets = 16 * sizeof(uint8_t)
public:
  Aes(Aes &&) = default;
  Aes(const Aes &) = default;
  typedef array<uint8_t, 4 * 4> key_t;
  typedef array<uint8_t, 4 * 4> block_t;
  Aes();
  template <typename T>
  vector<Aes::block_t> Encrypt(T, const key_t &, size_t = sizeof(T));

  template <typename T> T Decrypt(vector<Aes::block_t>, const key_t &);

  key_t gen_key();
  enum Encrypt_Mode {
    ECB,
    CBC,
  };
  Encrypt_Mode Mode_Enc = CBC;

  enum Padding_Mode {
    BASIC,
    RANDOM,
  };
  Padding_Mode Mode_Pad = BASIC;
  template <typename T> vector<block_t> inblock(T, size_t = sizeof(T));
  template <typename T> T fromblock(vector<block_t>);

  //* Encrpytion Mods
  void Encrypt_ECB(vector<Aes::block_t> &, const key_t &);
  void Decrypt_ECB(vector<Aes::block_t> &, const key_t &);

  void Decrypt_CBC(vector<Aes::block_t> &, const key_t &);
  void Encrypt_CBC(vector<Aes::block_t> &, const key_t &);
  //* Encrpytion Mods

private:
  array<uint8_t, 256> sbox;
  array<uint8_t, 256> un_sbox;
  // Initialisation of sbox and inverse
  void initialize_aes_sbox();

  array<int, 10> round_const;
  // Initialisation of round_consts
  void initialize_round_const();

  unordered_map<int, array<uint8_t, 256>>
      precalculated_multiples; //! Not used slow
  // Precalculate galoi multiplications table
  void initialize_multiplytables();

  const array<uint8_t, 4 * 4> MixMatrix = {2, 3, 1, 1, 1, 2, 3, 1,
                                           1, 1, 2, 3, 3, 1, 1, 2};
  const array<uint8_t, 4 * 4> Un_MixMatrix = {14, 11, 13, 9,  9,  14, 11, 13,
                                              13, 9,  14, 11, 11, 13, 9,  14};

  void block_encrypt(block_t &, const array<key_t, 11> &);
  //* Aes Block Crypt Procedures
  // ShiftRows Procedure on block
  void ShiftRows(block_t &);
  // MixColumns Procedure on block
  void MixColumns(block_t &);
  // Pass whole block through sbox
  void SubBlock(block_t &);
  //* Aes Block Crypt Procedures

  void block_decrypt(block_t &, const array<key_t, 11> &);
  //* Aes Block Decrypt Procedures
  // Aes Reverse ShiftRows Procedure on block
  void UnShiftRows(block_t &);
  // Aes Reverse MixColumns Procedure on block
  void UnMixColumns(block_t &);
  // Revert a whole block through un_sbox
  void UnSubBlock(block_t &);
  //* Aes Block Decrypt Procedures

  // AddRoundKey to block
  void AddRoundKey(block_t &, const key_t &);

  //* Aes Key Schedule Operations
  array<key_t, 11> ExpendKey(const key_t &);
  void RotWord(key_t &, int word_number);
  void SubWord(key_t &, int word_number);
  void XorWord(key_t &dst_key, int dst_word, key_t &, int nb_word_A, key_t &,
               int nb_word_B);
  void XorRecon(key_t &dst_key, int dst_word, key_t &key, int nb_word,
                int turn);
  //* Aes Key Schedule Operations

  //* Utils/Sous Functions
  uint8_t galoimul(uint8_t, uint8_t);
  pair<bitset<4>, bitset<4>> break_uint8(uint8_t);
  void SubCase(uint8_t *);
  void UnSubCase(uint8_t *);
  void ShiftRow(block_t &, const int line, int nbshifts);
  void UnMixColumn(block_t &, int nb_columns);
  void MixColumn(block_t &, int nb_columns);
  void Padding(block_t &, random_device &rand);

  // Debug Displays
  void DisplayState(const block_t &);
  void DisplayBoxs();
  //* Utils/Sous Functions
};
#endif // aes_hpp included
