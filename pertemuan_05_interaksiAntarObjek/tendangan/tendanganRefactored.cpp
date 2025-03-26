/* Nama File : tendanganRefactored.cpp
  Deskripsi : Program simulasi tendangan bola menggunakan OpenGL (refactored)
  Pembuat   : Sulhan Fuadi (24060123130115)
  Tanggal   : 26 Maret 2025
*/

#include <math.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PI 3.14

// Variabel-variabel untuk kamera
float angle = 0.0, deltaAngle = 0.0, ratio;
float x = -5.0f, y = 12.0f, z = 40.0f; // posisi awal kamera
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
int deltaMove = 0, h, w;

// Variabel-variabel untuk rotasi dan posisi
static int rotAngleX = 0, rotAngleY = 0, rotAngleZ = 0;
float posXKaki = 10, posXBola = -10, posYKaki = 6, posYBola = -5;
float rotKaki = 0.0;
int kick = 0, roll = 0, touch = 0;
float jarak = 1;

// Variabel-variabel untuk fitur tambahan
int score = 0;           // Skor pemain
float gravitasi = 0.005; // Konstanta gravitasi untuk efek lompatan bola
float kecBolaY = 0.0;    // Kecepatan bola pada sumbu Y
float posYAsli = -5.0;   // Posisi awal bola pada sumbu Y
int warnaBola = 0;       // Indeks warna bola saat ini

// Warna-warna untuk bola
const float warnaBolaPalet[][3] = {
    {0.8f, 0.4f, 0.0f}, // Oranye (default)
    {1.0f, 0.0f, 0.0f}, // Merah
    {0.0f, 0.7f, 0.0f}, // Hijau
    {0.0f, 0.0f, 1.0f}, // Biru
};
const int jumlahWarna = 4;

// Variabel untuk pencahayaan
GLUquadricObj *IDquadric;
const GLfloat light_ambient[] = {0.5f, 0.5f, 0.5f, 0.0f};
const GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_position[] = {0.0f, 20.0f, 10.0f, 1.0f};
const GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
const GLfloat mat_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
const GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat high_shininess[] = {100.0f};

// Fungsi inisialisasi dasar
void init(void)
{
  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  IDquadric = gluNewQuadric();              // Membuat pointer untuk objek kuadrik
  gluQuadricNormals(IDquadric, GLU_SMOOTH); // Membuat normal halus
  gluQuadricTexture(IDquadric, GL_TRUE);    // Mengaktifkan koordinat tekstur
}

// Fungsi untuk mengatur pencahayaan
void lighting()
{
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

// Fungsi reshape untuk mengatur perspektif kamera
void Reshape(int w1, int h1)
{
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

// Fungsi untuk memutar arah kamera (lihat kiri/kanan)
void orientMe(float ang)
{
  lx = sin(ang / 10);
  lz = -cos(ang / 10);
  glLoadIdentity();
  gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

// Fungsi untuk menggerakkan kamera maju/mundur
void moveMeFlat(int i)
{
  x = x + i * (lx) * 0.1;
  z = z + i * (lz) * 0.1;
  glLoadIdentity();
  gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

// Fungsi untuk memproses input keyboard
void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'w': // Putar objek ke atas
    rotAngleX += 2;
    break;
  case 's': // Putar objek ke bawah
    rotAngleX -= 2;
    break;
  case 'a': // Putar objek ke kiri
    rotAngleY += 2;
    break;
  case 'd': // Putar objek ke kanan
    rotAngleY -= 2;
    break;
  case 'q': // Putar objek searah jarum jam
    rotAngleZ += 2;
    break;
  case 'e': // Putar objek berlawanan arah jarum jam
    rotAngleZ -= 2;
    break;
  case 'o': // Gerakkan kaki ke kiri
    posXKaki -= 1;
    if (posXBola < -2.9) // Jika bola dekat, bola akan terdorong
    {
      posXBola += 1;
    }
    break;
  case 'p': // Gerakkan kaki ke kanan
    posXKaki += 1;
    posXBola -= 1;
    break;
  case 'k': // Mulai animasi tendangan
    kick = 1;
    break;
  case 'c': // Ganti warna bola
    warnaBola = (warnaBola + 1) % jumlahWarna;
    break;
  case 'r': // Reset skor
    score = 0;
    break;
  case 32: // Space bar - Reset semua posisi
    rotAngleX = rotAngleY = rotAngleZ = 0;
    posXKaki = 10;
    posXBola = -10;
    posYKaki = 6;
    posYBola = posYAsli;
    rotKaki = 0;
    kick = roll = 0;
    kecBolaY = 0.0;
    break;
  case 27: // Escape key - Keluar
    exit(0);
  default:
    break;
  }
  glutPostRedisplay();
}

// Fungsi untuk memproses tombol khusus (panah)
void pressKey(int k, int x, int y)
{
  switch (k)
  {
  case GLUT_KEY_UP: // Panah atas - gerakkan kamera maju
    deltaMove = 1;
    break;
  case GLUT_KEY_DOWN: // Panah bawah - gerakkan kamera mundur
    deltaMove = -1;
    break;
  case GLUT_KEY_LEFT: // Panah kiri - putar kamera ke kiri
    deltaAngle = -0.01f;
    break;
  case GLUT_KEY_RIGHT: // Panah kanan - putar kamera ke kanan
    deltaAngle = 0.01f;
    break;
  }
}

// Fungsi untuk memproses pelepasan tombol khusus
void releaseKey(int key, int x, int y)
{
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

// Fungsi untuk menggambar grid dasar
void Grid()
{
  double i;
  const float Z_MIN = -50, Z_MAX = 50;
  const float X_MIN = -50, X_MAX = 50;
  const float gap = 5;

  glColor3f(0.3, 0.3, 0.3);
  glBegin(GL_LINES);

  // Garis-garis sejajar sumbu Z
  for (i = Z_MIN; i < Z_MAX; i += gap)
  {
    glVertex3f(i, 0, Z_MIN);
    glVertex3f(i, 0, Z_MAX);
  }

  // Garis-garis sejajar sumbu X
  for (i = X_MIN; i < X_MAX; i += gap)
  {
    glVertex3f(X_MIN, 0, i);
    glVertex3f(X_MAX, 0, i);
  }

  glEnd();
}

// Fungsi untuk menggambar lapangan sederhana
void Lapangan()
{
  // Rumput lapangan (hijau)
  glColor3f(0.0f, 0.5f, 0.0f);
  glBegin(GL_QUADS);
  glVertex3f(-50, 0, 30);
  glVertex3f(-50, 0, -30);
  glVertex3f(50, 0, -30);
  glVertex3f(50, 0, 30);
  glEnd();

  // Garis-garis lapangan (putih)
  glColor3f(1.0f, 1.0f, 1.0f);

  // Garis tengah
  glBegin(GL_LINES);
  glVertex3f(0, 0.01, -30);
  glVertex3f(0, 0.01, 30);
  glEnd();

  // Garis tepi
  glBegin(GL_LINE_LOOP);
  glVertex3f(-50, 0.01, 30);
  glVertex3f(-50, 0.01, -30);
  glVertex3f(50, 0.01, -30);
  glVertex3f(50, 0.01, 30);
  glEnd();
}

// Fungsi untuk menggambar gawang
void Gawang()
{
  glPushMatrix();
  glTranslatef(-50, 0, 0);
  glRotatef(90, 0, 1, 0);

  // Tiang vertikal kiri
  glPushMatrix();
  glTranslatef(-5, 2.5, 0);
  glRotatef(90, 1, 0, 0);
  glColor3f(1.0f, 1.0f, 1.0f);
  gluCylinder(IDquadric, 0.2, 0.2, 5, 10, 10);
  glPopMatrix();

  // Tiang vertikal kanan
  glPushMatrix();
  glTranslatef(5, 2.5, 0);
  glRotatef(90, 1, 0, 0);
  gluCylinder(IDquadric, 0.2, 0.2, 5, 10, 10);
  glPopMatrix();

  // Tiang horizontal atas
  glPushMatrix();
  glTranslatef(0, 5, 0);
  glRotatef(90, 0, 1, 0);
  gluCylinder(IDquadric, 0.2, 0.2, 5, 10, 10);
  glPopMatrix();

  // Jaring gawang sederhana
  glColor3f(0.8f, 0.8f, 0.8f);
  glBegin(GL_LINES);

  // Garis horizontal jaring
  for (float y = 0.2; y < 5; y += 0.5)
  {
    glVertex3f(-5, y, 0);
    glVertex3f(5, y, 0);
  }

  // Garis vertikal jaring
  for (float x = -5; x <= 5; x += 0.5)
  {
    glVertex3f(x, 0.2, 0);
    glVertex3f(x, 5, 0);
  }

  glEnd();

  glPopMatrix();
}

// Fungsi untuk membuat balok (kaki)
void Balok(float panjang, float lebar, float tinggi)
{
  glPushMatrix();
  float p = panjang / 2;
  float l = lebar / 2;
  float t = tinggi / 2;

  // Sisi depan
  glBegin(GL_QUADS);
  glVertex3f(-p, 0, l);
  glVertex3f(p, 0, l);
  glVertex3f(p, -t * 2, l);
  glVertex3f(-p, -t * 2, l);
  glEnd();

  // Sisi belakang
  glBegin(GL_QUADS);
  glVertex3f(-p, 0, -l);
  glVertex3f(p, 0, -l);
  glVertex3f(p, -t * 2, -l);
  glVertex3f(-p, -t * 2, -l);
  glEnd();

  // Sisi atas
  glBegin(GL_QUADS);
  glVertex3f(-p, 0, -l);
  glVertex3f(p, 0, -l);
  glVertex3f(p, 0, l);
  glVertex3f(-p, 0, l);
  glEnd();

  // Sisi bawah
  glBegin(GL_QUADS);
  glVertex3f(-p, -t * 2, -l);
  glVertex3f(p, -t * 2, -l);
  glVertex3f(p, -t * 2, l);
  glVertex3f(-p, -t * 2, l);
  glEnd();

  // Sisi kanan
  glBegin(GL_QUADS);
  glVertex3f(-p, -t * 2, -l);
  glVertex3f(-p, -t * 2, l);
  glVertex3f(-p, 0, l);
  glVertex3f(-p, 0, -l);
  glEnd();

  // Sisi kiri
  glBegin(GL_QUADS);
  glVertex3f(p, -t * 2, -l);
  glVertex3f(p, -t * 2, l);
  glVertex3f(p, 0, l);
  glVertex3f(p, 0, -l);
  glEnd();

  glPopMatrix();
}

// Fungsi untuk menganimasi gerakan kaki
void pergerakanKaki()
{
  // Tahap 1: Menarik kaki ke belakang
  if (kick == 1)
  {
    if (rotKaki <= 45)
    {
      rotKaki += 0.03; // Kecepatan menarik kaki
    }
    if (rotKaki > 44.9)
    {
      kick = 2; // Lanjut ke tahap menendang
    }
  }

  // Deteksi apakah bola menempel pada kaki
  if (posXBola > -2.9)
  {
    touch = 1; // Bola menempel
  }
  else if (posXBola < -12)
  {
    touch = 0; // Bola tidak menempel
  }

  // Tahap 2: Mengayunkan kaki untuk menendang
  if (kick == 2)
  {
    if (rotKaki >= -90)
    {
      rotKaki -= 0.2; // Kecepatan mengayun kaki

      // Jika kaki menyentuh bola
      if (rotKaki < 1 && touch == 1)
      {
        roll = 1;       // Mulai pergerakan bola
        kecBolaY = 0.3; // Beri dorongan ke atas
      }
    }
    if (rotKaki < -90)
    {
      kick = 3; // Lanjut ke tahap mengembalikan kaki
    }
  }

  // Tahap 3: Mengembalikan posisi kaki
  if (kick == 3)
  {
    if (rotKaki <= 0)
    {
      rotKaki += 0.05; // Kecepatan kembali
    }
    if (rotKaki > -1)
    {
      kick = 0; // Selesai, kembali ke posisi awal
    }
  }
}

// Fungsi untuk menggerakkan bola
void pergerakanBola()
{
  // Efek gravitasi
  if (posYBola > posYAsli || kecBolaY != 0)
  {
    posYBola += kecBolaY;  // Gerakkan bola secara vertikal
    kecBolaY -= gravitasi; // Kurangi kecepatan karena gravitasi

    // Saat bola mencapai tanah
    if (posYBola < posYAsli)
    {
      posYBola = posYAsli;
      kecBolaY = -kecBolaY * 0.6; // Pantulan dengan redaman

      // Jika kecepatan kecil, hentikan gerakan
      if (fabs(kecBolaY) < 0.05)
      {
        kecBolaY = 0;
      }
    }
  }

  // Pergerakan horizontal bola setelah ditendang
  if (roll == 1)
  {
    if (jarak > 0)
    {
      posXBola -= 0.05; // Kecepatan horizontal bola
      jarak -= 0.005;   // Pengurangan jarak
    }
    else
    {
      roll = 0;
      jarak = 1;

      // Cek gol: jika posisi bola di area gawang
      if (posXBola < -52 && posYBola > 0 && posYBola < 5)
      {
        score++; // Tambah skor
        // Reset posisi bola
        posXBola = -10;
        posYBola = posYAsli;
        kecBolaY = 0;
      }
    }
  }
}

// Fungsi untuk menampilkan skor
void tampilkanSkor()
{
  char scoreStr[30];
  sprintf(scoreStr, "Skor: %d", score);

  // Atur proyeksi untuk teks 2D
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0.0, w, 0.0, h);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // Nonaktifkan pencahayaan untuk teks
  glDisable(GL_LIGHTING);

  // Tampilkan teks skor
  glColor3f(1.0f, 1.0f, 1.0f);
  glRasterPos2i(10, h - 20);

  for (int i = 0; scoreStr[i] != '\0'; i++)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreStr[i]);
  }

  // Aktifkan kembali pencahayaan
  glEnable(GL_LIGHTING);

  // Kembalikan matriks
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

// Fungsi untuk menampilkan bantuan kontrol di layar
void tampilkanBantuan()
{
  const char *bantuan[] = {
      "Kontrol:",
      "w/s - Putar atas/bawah",
      "a/d - Putar kiri/kanan",
      "o/p - Gerakkan kaki",
      "k - Tendang",
      "c - Ganti warna",
      "r - Reset skor",
      "space - Reset posisi"};

  int jumlahBaris = sizeof(bantuan) / sizeof(char *);

  // Atur proyeksi untuk teks 2D
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0.0, w, 0.0, h);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // Nonaktifkan pencahayaan untuk teks
  glDisable(GL_LIGHTING);

  // Tampilkan teks bantuan
  glColor3f(1.0f, 1.0f, 0.0f);

  for (int i = 0; i < jumlahBaris; i++)
  {
    glRasterPos2i(w - 150, h - 20 - (i * 15));
    for (int j = 0; bantuan[i][j] != '\0'; j++)
    {
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, bantuan[i][j]);
    }
  }

  // Aktifkan kembali pencahayaan
  glEnable(GL_LIGHTING);

  // Kembalikan matriks
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

// Fungsi untuk menggambar objek utama (kaki dan bola)
void DrawObjects()
{
  glPushMatrix();
  glTranslatef(posXKaki, posYKaki, 0);

  // Gambar kaki
  glPushMatrix();
  pergerakanKaki();
  glRotatef(rotKaki, 0, 0, 1); // Eksekusi rotasi kaki

  // Sepatu
  glColor3f(0.0, 0.0, 0.0);
  Balok(2.5, 3.5, 1.5);

  // Kaki bagian bawah
  glColor3f(0.8, 0.7, 0.6);
  glTranslatef(0, 2, 0);
  Balok(2, 3, 4);
  glPopMatrix();

  // Gambar bola
  glPushMatrix();
  pergerakanBola();
  glColor3f(warnaBolaPalet[warnaBola][0], warnaBolaPalet[warnaBola][1], warnaBolaPalet[warnaBola][2]);
  glTranslatef(posXBola, posYBola, 0);
  glutSolidSphere(1, 20, 20);

  // Garis-garis bola
  glColor3f(0.0, 0.0, 0.0);
  glutWireSphere(1.01, 8, 8);
  glPopMatrix();

  glPopMatrix();
}

// Fungsi display utama
void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pergerakan kamera jika tombol ditekan
  if (deltaMove)
    moveMeFlat(deltaMove);
  if (deltaAngle)
  {
    angle += deltaAngle;
    orientMe(angle);
  }

  glPushMatrix();
  // Rotasi seluruh adegan sesuai input
  glRotated(rotAngleX, 1, 0, 0);
  glRotated(rotAngleY, 0, 1, 0);
  glRotated(rotAngleZ, 0, 0, 1);

  // Gambar elemen-elemen adegan
  Grid();
  Lapangan();
  Gawang();
  DrawObjects();

  glPopMatrix();

  // Tampilkan UI
  tampilkanSkor();
  tampilkanBantuan();

  glutSwapBuffers();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Simulasi Tendangan Bola - Sulhan Fuadi");

  // Daftarkan fungsi callback
  glutSpecialFunc(pressKey);
  glutSpecialUpFunc(releaseKey);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(display);
  glutReshapeFunc(Reshape);

  // Inisialisasi
  lighting();
  init();

  // Tampilkan info di terminal
  printf("Simulasi Tendangan Bola\n");
  printf("======================\n");
  printf("- w/s: Putar atas/bawah\n");
  printf("- a/d: Putar kiri/kanan\n");
  printf("- q/e: Putar searah/berlawanan jarum jam\n");
  printf("- o/p: Gerakkan kaki kiri/kanan\n");
  printf("- k: Tendang bola\n");
  printf("- c: Ganti warna bola\n");
  printf("- r: Reset skor\n");
  printf("- Space: Reset posisi\n");
  printf("- Esc: Keluar\n");
  printf("======================\n");

  glutMainLoop();
  return (0);
}