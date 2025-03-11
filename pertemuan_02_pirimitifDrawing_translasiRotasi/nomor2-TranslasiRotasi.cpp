/* Nama File : nomor2-TranslasiRotasi
  Deskripsi : Program untuk menggambar perjalanan mobil 2D dari siang ke malam
  Pembuat   : Sulhan Fuadi (24060123130115)
  Tanggal   : 11 Maret 2025
*/

/*
Tugas Praktikum 3 nomor 2:
  Buatlah sebuah mobil 2D yang memanfaatkan object2 primitif dan transformasi (translasi dan rotasi).
  Penggunaan stack disarankan!
*/

/*
Saya menambahkan beberapa fungsi untuk menggambar perjalanan mobil 2D dari siang ke malam.
Fungsi-fungsi tersebut adalah:
  - gambarLingkaran(float jariJari, float pusatX, float pusatY, float r, float g, float b)
  - gambarLangit()
  - gambarJalan()
  - gambarBadanMobil()
  - gambarRoda()
  - gambarPemandangan()

Fungsi-fungsi tersebut digunakan untuk menggambar langit, jalan, mobil, roda, dan pemandangan.
Fungsi-fungsi tersebut dipanggil pada fungsi gambarAdegan untuk menggambar perjalanan mobil 2D.

Selain itu, saya juga menambahkan animasi perjalanan mobil 2D dari siang ke malam.
Fungsi animasi akan memperbarui posisi mobil, rotasi roda, waktu hari, dan lampu mobil.
Fungsi animasi dipanggil pada fungsi main untuk memulai animasi perjalanan mobil 2D.

Untuk penerapan stack, saya menggunakan glPushMatrix dan glPopMatrix pada fungsi-fungsi
gambarBadanMobil dan gambarRoda untuk menggambar badan mobil dan roda mobil.
Saya menggunakan glPushMatrix untuk menyimpan matriks modelview
sebelum melakukan translasi dan rotasi pada badan mobil dan roda mobil.
Saya menggunakan glPopMatrix untuk mengembalikan matriks modelview
ke kondisi sebelumnya setelah menggambar badan mobil dan roda mobil.
*/

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926535898
#define CIRCLE_POINTS 100

// Variabel global untuk animasi
float mobilX = -1.5;    // Posisi awal mobil di kiri layar
float rotasiRoda = 0.0; // Untuk rotasi roda
float waktuHari = 0.0;  // 0.0 = siang, 1.0 = malam
bool lampu = false;     // Status lampu mobil
float kecepatanMobil = 0.01;

// Fungsi untuk menggambar lingkaran
void gambarLingkaran(float jariJari, float pusatX, float pusatY, float r, float g, float b)
{
  glColor3f(r, g, b);
  glBegin(GL_POLYGON);
  for (int i = 0; i < CIRCLE_POINTS; i++)
  {
    float sudut = 2.0 * PI * i / CIRCLE_POINTS;
    float x = pusatX + jariJari * cos(sudut);
    float y = pusatY + jariJari * sin(sudut);
    glVertex2f(x, y);
  }
  glEnd();
}

// Fungsi untuk menggambar langit berdasarkan waktu
void gambarLangit()
{
  // Warna langit berubah dari biru (siang) ke biru gelap (malam)
  float biru = 1.0 - (waktuHari * 0.8);
  float merah = 0.3 - (waktuHari * 0.3);
  float hijau = 0.7 - (waktuHari * 0.7);

  glBegin(GL_QUADS);
  glColor3f(merah, hijau, biru);
  glVertex2f(-1.0, 1.0);
  glVertex2f(1.0, 1.0);
  glVertex2f(1.0, 0.0);
  glVertex2f(-1.0, 0.0);
  glEnd();

  // Jika malam, gambar bulan. Jika siang, gambar matahari
  if (waktuHari > 0.5)
  {
    // Bulan
    gambarLingkaran(0.1, 0.7, 0.8, 0.9, 0.9, 0.7);
  }
  else
  {
    // Matahari
    gambarLingkaran(0.1, 0.7, 0.8, 1.0, 1.0, 0.0);
  }
}

// Fungsi untuk menggambar jalan
void gambarJalan()
{
  // Jalan
  glBegin(GL_QUADS);
  glColor3f(0.2, 0.2, 0.2);
  glVertex2f(-1.0, 0.0);
  glVertex2f(1.0, 0.0);
  glVertex2f(1.0, -0.5);
  glVertex2f(-1.0, -0.5);
  glEnd();

  // Garis jalan
  glBegin(GL_QUADS);
  glColor3f(1.0, 1.0, 1.0);

  // Beberapa garis putih di jalan
  for (float x = -0.9; x < 1.0; x += 0.3)
  {
    glVertex2f(x, -0.24);
    glVertex2f(x + 0.15, -0.24);
    glVertex2f(x + 0.15, -0.26);
    glVertex2f(x, -0.26);
  }
  glEnd();
}

// Fungsi untuk menggambar badan mobil
void gambarBadanMobil()
{
  // Badan utama mobil
  glBegin(GL_POLYGON);
  glColor3f(1.0, 0.0, 0.0); // Mobil merah
  glVertex2f(0.0, -0.15);
  glVertex2f(0.4, -0.15);
  glVertex2f(0.35, -0.05);
  glVertex2f(0.3, 0.1);
  glVertex2f(0.05, 0.1);
  glVertex2f(0.0, -0.05);
  glEnd();

  // Kaca depan
  glBegin(GL_POLYGON);
  glColor3f(0.7, 0.9, 1.0); // Kaca biru muda
  glVertex2f(0.05, 0.1);
  glVertex2f(0.3, 0.1);
  glVertex2f(0.25, 0.05);
  glVertex2f(0.1, 0.05);
  glEnd();

  // Kaca samping
  glBegin(GL_POLYGON);
  glColor3f(0.7, 0.9, 1.0);
  glVertex2f(0.1, 0.05);
  glVertex2f(0.25, 0.05);
  glVertex2f(0.25, -0.05);
  glVertex2f(0.1, -0.05);
  glEnd();

  // Lampu mobil
  if (lampu)
  {
    // Sinar lampu saat malam
    glBegin(GL_TRIANGLES);
    glColor4f(1.0, 1.0, 0.6, 0.3); // Warna kuning transparan
    glVertex2f(0.4, -0.12);
    glVertex2f(0.7, -0.05);
    glVertex2f(0.7, -0.2);
    glEnd();
  }

  // Lampu depan mobil
  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 0.0); // Kuning
  glVertex2f(0.38, -0.12);
  glVertex2f(0.4, -0.12);
  glVertex2f(0.4, -0.15);
  glVertex2f(0.38, -0.15);
  glEnd();
}

// Fungsi untuk menggambar roda dengan efek rotasi
void gambarRoda()
{
  glPushMatrix();
  // Roda belakang
  glTranslatef(0.1, -0.15, 0.0);
  glRotatef(rotasiRoda, 0.0, 0.0, 1.0);           // Rotasi roda
  gambarLingkaran(0.05, 0.0, 0.0, 0.1, 0.1, 0.1); // Roda hitam

  // Detail roda (jari-jari)
  glBegin(GL_LINES);
  glColor3f(0.5, 0.5, 0.5);
  for (int i = 0; i < 8; i++)
  {
    float sudut = PI / 4 * i;
    glVertex2f(0.0, 0.0);
    glVertex2f(0.04 * cos(sudut), 0.04 * sin(sudut));
  }
  glEnd();
  glPopMatrix();

  glPushMatrix();
  // Roda depan
  glTranslatef(0.3, -0.15, 0.0);
  glRotatef(rotasiRoda, 0.0, 0.0, 1.0);           // Rotasi roda
  gambarLingkaran(0.05, 0.0, 0.0, 0.1, 0.1, 0.1); // Roda hitam

  // Detail roda (jari-jari)
  glBegin(GL_LINES);
  glColor3f(0.5, 0.5, 0.5);
  for (int i = 0; i < 8; i++)
  {
    float sudut = PI / 4 * i;
    glVertex2f(0.0, 0.0);
    glVertex2f(0.04 * cos(sudut), 0.04 * sin(sudut));
  }
  glEnd();
  glPopMatrix();
}

// Fungsi untuk menggambar pemandangan
void gambarPemandangan()
{
  // Bukit di kejauhan
  glBegin(GL_POLYGON);
  glColor3f(0.0, 0.4 - (waktuHari * 0.3), 0.0); // Hijau menjadi gelap di malam hari
  glVertex2f(-1.0, 0.0);
  glVertex2f(-0.8, 0.1);
  glVertex2f(-0.6, 0.05);
  glVertex2f(-0.4, 0.15);
  glVertex2f(-0.2, 0.05);
  glVertex2f(0.0, 0.1);
  glVertex2f(0.2, 0.05);
  glVertex2f(0.4, 0.15);
  glVertex2f(0.6, 0.05);
  glVertex2f(0.8, 0.1);
  glVertex2f(1.0, 0.0);
  glEnd();
}

// Fungsi untuk menggambar seluruh adegan
void gambarAdegan(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  gambarLangit();
  gambarPemandangan();
  gambarJalan();

  // Menggambar mobil dengan translasi
  glPushMatrix();
  glTranslatef(mobilX, 0.0, 0.0);
  gambarBadanMobil();
  gambarRoda();
  glPopMatrix();

  glutSwapBuffers();
}

// Fungsi untuk animasi
void animasi(int value)
{
  // Update posisi mobil
  mobilX += kecepatanMobil;

  // Reset posisi jika keluar dari layar
  if (mobilX > 1.5)
  {
    mobilX = -1.5;
  }

  // Update rotasi roda
  rotasiRoda += 10.0;
  if (rotasiRoda > 360.0)
  {
    rotasiRoda -= 360.0;
  }

  // Update waktu hari
  waktuHari += 0.002;
  if (waktuHari > 1.0)
  {
    waktuHari = 0.0;
  }

  // Nyalakan lampu mobil jika malam
  lampu = (waktuHari > 0.5);

  glutPostRedisplay();           // Minta GLUT untuk menggambar ulang
  glutTimerFunc(30, animasi, 0); // Panggil fungsi animasi lagi setelah 30ms
}

// Fungsi utama, digunakan untuk menjalankan program
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // Double buffer untuk animasi
  glutCreateWindow("Tugas Praktikum 3: Translasi dan Rotasi (Nomor 2) - Mobil Gabut");
  glutDisplayFunc(gambarAdegan);
  glutTimerFunc(30, animasi, 0); // Mulai animasi
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glutMainLoop();
  return 0;
}
