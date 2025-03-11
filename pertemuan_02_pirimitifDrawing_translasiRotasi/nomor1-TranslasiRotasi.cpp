/* Nama File : nomor1-TranslasiRotasi.cpp
  Deskripsi : Program untuk membuat lingkaran yang berputar di sekitar lingkaran lain
  Pembuat   : Sulhan Fuadi (24060123130115)
  Tanggal   : 11 Maret 2025
*/

/*
Tugas Praktikum 3 nomor 1:
  Buatlah sebuah lingkaran.

  Cerita Latar Belakang Code:
  Dalam keluarga Tata Surya Mini, Matahari (lingkaran kuning) bersinar terang
  sementara planet kecil (lingkaran biru) setia mengorbit mengelilinginya.
  Di sekitar mereka bertebaran bintang-bintang kecil yang menyaksikan
  perjalanan tanpa henti sang planet.
*/

/*
Saya menambahkan beberapa fungsi untuk menggambar lingkaran dan bintang-bintang
dengan warna yang berbeda-beda. Fungsi-fungsi tersebut adalah:
  - gambarLingkaran(float xPusat, float yPusat, float radius, int potongan)
  - inisialisasiBintang()

Fungsi-fungsi tersebut digunakan untuk menggambar lingkaran dan bintang-bintang
pada tata surya mini. Fungsi-fungsi tersebut dipanggil pada fungsi RenderScene
untuk menggambar lingkaran dan bintang-bintang tersebut.

Selain itu, saya juga menambahkan animasi rotasi pada planet dengan menggunakan
fungsi update. Fungsi update akan memperbarui sudut rotasi planet setiap kali
scene di-render ulang. Fungsi update dipanggil pada fungsi main untuk mengupdate
animasi rotasi planet.
*/

#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.1415926535898
#define circle_points 100

// Variabel untuk animasi
float rotasiPlanet = 0.0f;
float kecepatanRotasi = 0.05f;

// Struktur untuk bintang
struct Bintang
{
  float x, y;
  float ukuran;
  float kecerahan;
};

// Array untuk menyimpan bintang-bintang
#define JUMLAH_BINTANG 30
Bintang bintang[JUMLAH_BINTANG];

// Fungsi untuk menggambar lingkaran
void gambarLingkaran(float xPusat, float yPusat, float radius, int potongan)
{
  float sudut, x, y;
  glBegin(GL_POLYGON);
  for (int i = 0; i <= potongan; i++)
  {
    sudut = i * (2 * PI / potongan);
    x = xPusat + radius * cos(sudut);
    y = yPusat + radius * sin(sudut);
    glVertex2f(x, y);
  }
  glEnd();
}

// Fungsi untuk menginisialisasi bintang-bintang
void inisialisasiBintang()
{
  // Inisialisasi posisi bintang secara acak
  for (int i = 0; i < JUMLAH_BINTANG; i++)
  {
    bintang[i].x = (float)(rand() % 200 - 100) / 100.0f;
    bintang[i].y = (float)(rand() % 200 - 100) / 100.0f;
    bintang[i].ukuran = (float)(rand() % 5 + 1) / 100.0f;
    bintang[i].kecerahan = (float)(rand() % 100) / 100.0f;
  }
}

void RenderScene(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  // Menggambar latar belakang hitam (langit malam)
  glClearColor(0.0f, 0.0f, 0.1f, 1.0f);

  // Menggambar bintang-bintang
  for (int i = 0; i < JUMLAH_BINTANG; i++)
  {
    glPushMatrix();
    glColor4f(1.0f, 1.0f, 1.0f, bintang[i].kecerahan);
    gambarLingkaran(bintang[i].x, bintang[i].y, bintang[i].ukuran, 8);
    glPopMatrix();
  }

  // Menggambar matahari (pusat)
  glPushMatrix();
  glColor3f(1.0f, 0.8f, 0.0f);                       // Warna kuning matahari
  gambarLingkaran(0.0f, 0.0f, 0.25f, circle_points); // Matahari di tengah
  glPopMatrix();

  // Menggambar orbit planet
  glPushMatrix();
  glColor4f(0.5f, 0.5f, 0.5f, 0.2f); // Jalur orbit semi-transparan
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i <= circle_points; i++)
  {
    float sudut = i * (2 * PI / circle_points);
    float x = 0.6f * cos(sudut);
    float y = 0.6f * sin(sudut);
    glVertex2f(x, y);
  }
  glEnd();
  glPopMatrix();

  // Menggambar planet yang berputar
  glPushMatrix();
  // Translasi planet ke posisi orbit berdasarkan sudut rotasi
  float xPlanet = 0.6f * cos(rotasiPlanet);
  float yPlanet = 0.6f * sin(rotasiPlanet);
  glTranslatef(xPlanet, yPlanet, 0.0f);

  // Warna biru untuk planet
  glColor3f(0.0f, 0.4f, 0.8f);
  gambarLingkaran(0.0f, 0.0f, 0.08f, circle_points);
  glPopMatrix();

  glutSwapBuffers();
}

// Fungsi untuk mengupdate animasi
// setiap kali scene di-render ulang, fungsi ini akan dipanggil
void update(void)
{
  // Memperbarui sudut rotasi planet
  rotasiPlanet += kecepatanRotasi * 0.01f;
  if (rotasiPlanet > 2 * PI)
    rotasiPlanet -= 2 * PI;

  // Meminta render ulang scene
  glutPostRedisplay();
}

// Fungsi utama, digunakan untuk menjalankan program
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);

  // Menggunakan double buffer untuk animasi yang halus
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

  glutCreateWindow("Tugas Praktikum 3: Translasi dan Rotasi (Nomor 1) - Tata Surya Mini");

  // Inisialisasi bintang sebelum menggambar
  inisialisasiBintang();

  glutDisplayFunc(RenderScene);

  // Mendaftarkan fungsi update untuk animasi
  glutIdleFunc(update);

  // Warna latar belakang hitam untuk langit malam
  glClearColor(0.0f, 0.0f, 0.1f, 1.0f);

  glutMainLoop();
  return 0;
}