#include <_types/_uint8_t.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#define __OSX__
#ifndef __OSX__
#include <GL/gl.h>
#else
/* pour mac */
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#endif
#include "Lib/aes.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "Lib/stb_image.h"
struct Image {
  int sizeX;
  int sizeY;
  GLubyte *data;
};
typedef struct Image Image;

Image *image;
Aes Aes_ctx = Aes();
Aes::key_t current_key;
vector<Aes::block_t> blocks;
bool encrypted = false;

void swap(GLubyte *A, GLubyte *B) {
  GLubyte tmp = *A;
  *A = *B;
  *B = tmp;
}

void inverse(Image *i) {
  int size, j;
  GLubyte *red, *green, *blue;
  int moyenne;
  GLubyte *ptr, *end_ptr;
  size = i->sizeY * i->sizeX;
  for (ptr = i->data, end_ptr = ptr + ((4 * size) - 4); end_ptr > ptr;
       ptr += 4, end_ptr -= 4) {
    swap(ptr, end_ptr);
    swap(ptr + 1, end_ptr + 1);
    swap(ptr + 2, end_ptr + 2);
    swap(ptr + 3, end_ptr + 3);
  }
}

int Init(char *filename) {
  image = (Image *)malloc(sizeof(Image));
  image->data = stbi_load(filename, &image->sizeX, &image->sizeY, NULL, 4);
  if (image->data == NULL) {
    cerr << "Error Image Not Found or wrong format" << endl;
    exit(-1);
  }
  glutReshapeWindow(image->sizeX, image->sizeY);
  inverse(image);
  return (0);
}

void Display(void) {
  GLint w, h;
  glClear(GL_COLOR_BUFFER_BIT);
  w = glutGet(GLUT_WINDOW_WIDTH);
  h = glutGet(GLUT_WINDOW_HEIGHT);
  glDrawPixels(image->sizeX, image->sizeY, GL_RGBA, GL_UNSIGNED_BYTE,
               image->data);
  glFlush();
}

void Reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void ECB() {
  Aes_ctx.Mode_Enc = Aes::ECB;
  current_key = Aes_ctx.gen_key();
  vector<Aes::block_t> current_blocks;
  blocks = current_blocks;
  uint8_t *tmp = (uint8_t *)malloc(sizeof(uint8_t) * 4);
  for (size_t i = 0; i < image->sizeX * image->sizeY * 4; i += 4) {
    current_blocks =
        Aes_ctx.Encrypt(&image->data[i], current_key, sizeof(uint8_t) * 4);
    tmp = Aes_ctx.fromblock<uint8_t *>(current_blocks);
    image->data[i] = tmp[0];
    image->data[i + 1] = tmp[1];
    image->data[i + 2] = tmp[2];
    image->data[i + 3] = tmp[3];
    for (size_t j = 0; j < current_blocks.size(); j++)
      blocks.push_back(current_blocks[j]);
  }
  encrypted = true;
}

void CBC() {
  Aes_ctx.Mode_Enc = Aes::CBC;
  current_key = Aes_ctx.gen_key();
  blocks =
      Aes_ctx.Encrypt(image->data, current_key,
                      sizeof(image->data) * (image->sizeX * image->sizeY) * 4);
  GLubyte *tmp = image->data;
  image->data = Aes_ctx.fromblock<GLubyte *>(blocks);
  free(tmp);
  encrypted = true;
}

void Decrypt() {
  if (Aes_ctx.Mode_Enc == Aes::ECB) {
    vector<Aes::block_t> current_blocks(3);
    uint8_t *tmp = (uint8_t *)malloc(sizeof(uint8_t) * 4);
    tmp = Aes_ctx.Decrypt<uint8_t *>(blocks, current_key);
    int t = 0;
    for (size_t i = 0; i < blocks.size() * 16; i++) {
      if (i % 16 == 0) {
        image->data[t] = tmp[i];
        image->data[t + 1] = tmp[i + 1];
        image->data[t + 2] = tmp[i + 2];
        image->data[i + 3] = tmp[3];
        t += 4;
      }
    }
  } else {
    GLubyte *tmp = image->data;
    image->data = Aes_ctx.Decrypt<GLubyte *>(blocks, current_key);
    free(tmp);
  }
  encrypted = false;
}

void menuFunc(int item) {
  switch (item) {
  case 0:
    free(image->data);
    free(image);
    exit(0);
    break;
  case 1:
    if (!encrypted) {
      ECB();
      Display();
    }
    break;
  case 2:
    if (!encrypted) {
      CBC();
      Display();
    }
    break;
  case 3:
    if (encrypted) {
      Decrypt();
      Display();
    }
    break;
  default:
    break;
  }
}

void Keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27:
    exit(0);
    break;
  default:
    fprintf(stderr, "Unused key\n");
  }
}

void Mouse(int button, int state, int x, int y) {

  switch (button) {
  case GLUT_LEFT_BUTTON:
    break;
  case GLUT_MIDDLE_BUTTON:
    break;
  case GLUT_RIGHT_BUTTON:
    break;
  }
  glutPostRedisplay();
}

int main(int argc, char **argv) {

  if (argc < 2) {
    fprintf(stderr, "Usage : Aes_img nom_de_fichier Padding_Mode\n");
    exit(0);
  }
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
  glutInitWindowSize(GLUT_WINDOW_WIDTH, GLUT_WINDOW_HEIGHT);
  glutCreateWindow("Aes_img");

  glutCreateMenu(menuFunc);
  glutAddMenuEntry("Quit", 0);
  glutAddMenuEntry("Encrypt ECB", 1);
  glutAddMenuEntry("Encrypt CBC", 2);
  glutAddMenuEntry("Decrypt", 3);

  glutAttachMenu(GLUT_LEFT_BUTTON);

  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);

  Init(argv[1]);
  glutMouseFunc(Mouse);
  glutMainLoop();
  return 1;
}