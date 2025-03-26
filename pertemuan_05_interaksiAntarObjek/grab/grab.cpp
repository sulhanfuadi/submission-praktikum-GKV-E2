/* Nama File : grab.cpp
  Deskripsi : Program simulasi robot mengambil kotak menggunakan OpenGL
  Pembuat   : Sulhan Fuadi (24060123130115)
  Tanggal   : 26 Maret 2025
*/

#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PI 3.14

float angle = 0.0, deltaAngle = 0.0, ratio;
float x = 5.0f, y = 10.0f, z = 40.0f; // posisi awal kamera
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
int deltaMove = 0, h, w;
static int rotAngleX = 0, rotAngleY = 0, rotAngleZ = 0;

// Posisi objek
float posXBadan = 10, posXKotak = 0, posYBadan = 7, posYKotak = 6;
float rotTangan1 = 0.0, rotTangan2 = 0.0, rotTangan3 = 0.0, rotTangan4 = 0.0;

// Status interaksi
int kick = 0, roll = 0, hit = 0, gerakTangan = 0, drop = 0, bring = 0, grab = 0, tabrak = 0;

// Batas collision
float jarakAmbil = 3.0;      // Jarak maksimum untuk mengambil kotak
float toleransiTabrak = 1.0; // Toleransi untuk tabrakan

// Fisika
float kecepatanJatuh = 0.1; // Kecepatan jatuh kotak
float batasLantai = 3.0;    // Posisi Y lantai untuk kotak
float posXMeja = 0.0;       // Posisi X meja
float lebarMeja = 5.0;      // Lebar meja

GLUquadricObj *IDquadric;

// Variable untuk pencahayaan
const GLfloat light_ambient[] = {0.5f, 0.5f, 0.5f, 0.0f};
const GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_position[] = {0.0f, 20.0f, 10.0f, 1.0f};
const GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
const GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
const GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat high_shininess[] = {100.0f};

void init(void)
{
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  IDquadric = gluNewQuadric();              // Create A Pointer To The Quadric Object
  gluQuadricNormals(IDquadric, GLU_SMOOTH); // Create Smooth Normals
  gluQuadricTexture(IDquadric, GL_TRUE);    // Create Texture Coords
}

void Reshape(int w1, int h1)
{
  // Fungsi reshape
  if (h1 == 0)
    h1 = 1;
  w = w1;
  h = h1;
  ratio = 1.0f * w / h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h);
  gluPerspective(45, ratio, 0.1, 1000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

void orientMe(float ang)
{
  // Fungsi ini untuk memutar arah kamera (tengok kiri/kanan)
  lx = sin(ang / 10);
  lz = -cos(ang / 10);
  glLoadIdentity();
  gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

void moveMeFlat(int i)
{
  // Fungsi ini untuk maju mundur kamera
  x = x + i * (lx) * 0.1;
  z = z + i * (lz) * 0.1;
  glLoadIdentity();
  gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

// Fungsi untuk mengecek apakah robot berada dalam jarak untuk mengambil kotak
bool dalamJarakAmbil()
{
  float jarakX = fabs(posXBadan - posXKotak);
  return jarakX <= jarakAmbil;
}

// Fungsi untuk mengecek apakah robot akan menabrak meja
bool akanTabrakMeja(float pergeseran)
{
  float posXBaruBadan = posXBadan + pergeseran;
  float jarakKeMeja = fabs(posXBaruBadan - posXMeja);
  return jarakKeMeja < lebarMeja / 2 + 1.5 && posYKotak >= batasLantai + 1.0;
}

// Fungsi untuk mengecek apakah robot akan menabrak kotak
bool akanTabrakKotak(float pergeseran)
{
  if (drop == 0 || grab == 1)
    return false; // Jika kotak belum jatuh atau sedang dipegang, tidak ada tabrakan

  float posXBaruBadan = posXBadan + pergeseran;
  float jarakKeKotak = fabs(posXBaruBadan - posXKotak);
  return jarakKeKotak < 3.0 + toleransiTabrak && posYKotak <= batasLantai + 0.1;
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'w':
    rotAngleX += 2;
    break;
  case 's':
    rotAngleX -= 2;
    break;
  case 'a':
    rotAngleY += 2;
    break;
  case 'd':
    rotAngleY -= 2;
    break;
  case 'q':
    rotAngleZ += 2;
    break;
  case 'e':
    rotAngleZ -= 2;
    break;
  case 'o': // Gerak ke kiri
    if (!akanTabrakMeja(-1) && !akanTabrakKotak(-1))
    {
      posXBadan -= 1;
      if (bring == 1)
      {
        posXKotak -= 1;
      }
    }
    break;
  case 'p': // Gerak ke kanan
    posXBadan += 1;
    if (bring == 1)
    {
      posXKotak += 1;
    }
    break;
  case 'g': // Perintah untuk grab
    if (gerakTangan == 0 && dalamJarakAmbil())
    {
      gerakTangan = 1;
      hit = 1; // Tandai bahwa bisa mengambil kotak
    }
    break;
  case 't': // Perintah untuk drop
    if (bring == 1)
    { // Hanya bisa menjatuhkan jika sedang membawa
      drop = 1;
      gerakTangan = 3; // Mulai proses melepaskan
    }
    break;
  case 32: // Spasi - Reset
    rotAngleX = rotAngleY = rotAngleZ = 0;
    posXBadan = 10;
    posXKotak = 0;
    posYBadan = 7;
    posYKotak = 6;
    rotTangan1 = rotTangan2 = rotTangan3 = rotTangan4 = 0;
    kick = roll = gerakTangan = drop = hit = bring = grab = 0;
    break;
  case 27: // ESC - Keluar
    exit(0);
  default:
    break;
  }
  glutPostRedisplay();
}

void pressKey(int k, int x, int y)
{
  // Fungsi ini akan dijalankan saat tombol keyboard ditekan dan belum dilepas
  // Selama tombol ditekan, variabel angle dan move diubah => kamera bergerak
  switch (k)
  {
  case GLUT_KEY_UP:
    deltaMove = 1;
    break;
  case GLUT_KEY_DOWN:
    deltaMove = -1;
    break;
  case GLUT_KEY_LEFT:
    deltaAngle = -0.01f;
    break;
  case GLUT_KEY_RIGHT:
    deltaAngle = 0.01f;
    break;
  }
}

void releaseKey(int key, int x, int y)
{
  // Fungsi ini akan dijalankan saat tekanan tombol keyboard dilepas
  switch (key)
  {
  case GLUT_KEY_UP:
    if (deltaMove > 0)
      deltaMove = 0;
    break;
  case GLUT_KEY_DOWN:
    if (deltaMove < 0)
      deltaMove = 0;
    break;
  case GLUT_KEY_LEFT:
    if (deltaAngle < 0.0f)
      deltaAngle = 0.0f;
    break;
  case GLUT_KEY_RIGHT:
    if (deltaAngle > 0.0f)
      deltaAngle = 0.0f;
    break;
  }
}

void lighting()
{
  // Fungsi mengaktifkan pencahayaan
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

void Grid()
{
  // Fungsi untuk membuat grid di "lantai"
  double i;
  const float Z_MIN = -50, Z_MAX = 50;
  const float X_MIN = -50, X_MAX = 50;
  const float gap = 2;
  glColor3f(0.5, 0.5, 0.5);
  glBegin(GL_LINES);
  for (i = Z_MIN; i < Z_MAX; i += gap)
  {
    glVertex3f(i, 0, Z_MIN);
    glVertex3f(i, 0, Z_MAX);
  }
  for (i = X_MIN; i < X_MAX; i += gap)
  {
    glVertex3f(X_MIN, 0, i);
    glVertex3f(X_MAX, 0, i);
  }
  glEnd();
}

void Grid2()
{
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_QUADS);
  glVertex3f(-50, 0, 50);
  glVertex3f(-50, 0, -50);
  glVertex3f(50, 0, -50);
  glVertex3f(50, 0, 50);
  glEnd();
}

void Balok(float panjang, float lebar, float tinggi)
{
  glPushMatrix();
  float p = panjang / 2;
  float l = lebar / 2;
  float t = tinggi / 2;

  // depan
  glBegin(GL_QUADS);
  glVertex3f(-p, 0, l);
  glVertex3f(p, 0, l);
  glVertex3f(p, -t * 2, l);
  glVertex3f(-p, -t * 2, l);
  glEnd();

  // belakang
  glBegin(GL_QUADS);
  glVertex3f(-p, 0, -l);
  glVertex3f(p, 0, -l);
  glVertex3f(p, -t * 2, -l);
  glVertex3f(-p, -t * 2, -l);
  glEnd();

  // atas
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-p, 0, -l);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(p, 0, -l);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(p, 0, l);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-p, 0, l);
  glEnd();

  // bawah
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-p, -t * 2, -l);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(p, -t * 2, -l);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(p, -t * 2, l);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-p, -t * 2, l);
  glEnd();

  // kanan
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-p, -t * 2, -l);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-p, -t * 2, l);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-p, 0, l);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-p, 0, -l);
  glEnd();

  // kiri
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(p, -t * 2, -l);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f(p, -t * 2, l);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(p, 0, l);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f(p, 0, -l);
  glEnd();
  glPopMatrix();
}

// Prosedur untuk mengupdate posisi kotak
void perubahKotak()
{
  if (drop == 1 && grab == 1)
  {
    // Jatuhkan kotak jika perintah drop diberikan saat kotak dipegang
    if (posYKotak > batasLantai)
    {
      posYKotak -= kecepatanJatuh; // Kecepatan jatuh yang lebih realistis
    }

    if (posYKotak <= batasLantai)
    {
      posYKotak = batasLantai; // Pastikan tidak menembus lantai
      // Reset semua status interaksi
      bring = 0;
      hit = 0;
      grab = 0;
    }
  }
}

// Prosedur untuk mengubah gerakan tangan dan mendeteksi interaksi
void pengubahTangan()
{
  // Deteksi apakah robot berada dalam jarak untuk mengambil kotak
  if (dalamJarakAmbil())
  {
    hit = 1;
  }
  else
  {
    hit = 0;
  }

  // Jika menyentuh kotak dan ada perintah mengambil, bawa kotak
  if (hit == 1 && grab == 1)
  {
    bring = 1; // Memegang kotak
  }

  // Gerakan tangan (mekanisme 4 tahap)
  if (gerakTangan == 1)
  {
    // Tahap 1: Pelebaran tangan samping
    if (rotTangan1 > -90)
    {
      rotTangan1 -= 2.0; // Kecepatan rotasi yang lebih cepat
    }

    if (rotTangan1 <= -90)
    {
      gerakTangan = 2; // Lanjut ke tahap berikutnya
    }
  }

  if (gerakTangan == 2)
  {
    // Tahap 2: Merapatkan tangan depan (grab)
    if (rotTangan2 > -90)
    {
      rotTangan2 -= 2.0;
    }

    if (rotTangan2 <= -90)
    {
      if (hit == 1)
      {
        grab = 1; // Ambil kotak jika dalam jangkauan
      }
      // Jika tidak berhasil mengambil, tetap lanjut ke tahap 3
      if (grab == 0 && rotTangan2 <= -90)
      {
        gerakTangan = 3;
      }
    }
  }

  if (gerakTangan == 3)
  {
    // Tahap 3: Melebarkan tangan depan (release)
    if (rotTangan2 < 0)
    {
      rotTangan2 += 2.0;
    }

    if (rotTangan2 >= 0)
    {
      gerakTangan = 4; // Lanjut ke tahap berikutnya
    }
  }

  if (gerakTangan == 4)
  {
    // Tahap 4: Merapatkan tangan samping (kembali ke posisi awal)
    if (rotTangan1 < 0)
    {
      rotTangan1 += 2.0;
    }

    if (rotTangan1 >= 0)
    {
      gerakTangan = 0; // Selesai, kembali ke posisi normal
    }
  }
}

void Object()
{
  // Meja
  glPushMatrix();
  glColor3f(0.1, 0.1, 0.2);
  glTranslatef(posXMeja, 3, 0);
  Balok(lebarMeja, 5, 3);
  glPopMatrix();

  // Kotak pink
  glPushMatrix();
  perubahKotak(); // Update posisi kotak
  glColor3f(0.8, 0.3, 0.3);
  glTranslatef(posXKotak, posYKotak, 0);
  Balok(3, 3, 3);
  glPopMatrix();

  // Robot dan tangan
  glPushMatrix();
  pengubahTangan(); // Update gerakan tangan

  // Badan robot
  glColor3f(0.3, 0.3, 0.8);
  glTranslatef(posXBadan, posYBadan, 0);
  Balok(3, 3, 7);

  // Tangan kiri
  glPushMatrix();
  glColor3f(0.2, 0.5, 0.2);
  glTranslatef(0, -2, 2.5);
  glRotatef(rotTangan1, 1, 0, 0);
  glRotatef(rotTangan2, 0, 0, 1);
  Balok(2, 2, 4);
  glPopMatrix();

  // Tangan kanan
  glPushMatrix();
  glColor3f(0.2, 0.5, 0.2);
  glTranslatef(0, -2, -2.5);
  glRotatef(-rotTangan1, 1, 0, 0);
  glRotatef(rotTangan2, 0, 0, 1);
  Balok(2, 2, 4);
  glPopMatrix();

  glPopMatrix();
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Kalau move dan angle tidak nol, gerakkan kamera...
  if (deltaMove)
    moveMeFlat(deltaMove);
  if (deltaAngle)
  {
    angle += deltaAngle;
    orientMe(angle);
  }

  glPushMatrix();
  glRotated(rotAngleX + 10, 1, 0, 0);
  glRotated(rotAngleY, 0, 1, 0);
  glRotated(rotAngleZ, 0, 0, 1);

  // Gambar grid
  Grid();
  Grid2();

  // Gambar objek
  Object();

  glPopMatrix();

  glFlush();
  glutSwapBuffers();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Grab");

  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(display);
  glutReshapeFunc(Reshape);

  lighting();
  init();
  glutMainLoop();
  return (0);
}