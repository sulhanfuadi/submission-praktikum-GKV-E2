/* Nama File : nomor1-PrimitifDrawing.cpp
  Deskripsi : Program menampilkan primitif dasar OpenGL
  Pembuat   : Sulhan Fuadi (24060123130115)
  Tanggal   : 09 Maret 2025
*/

/*
Tugas Praktikum 2 nomor 1:
  Selain membuat titik menggunakan GL_POINT, garis menggunakan GL_LINES,
  buatlah program yang memanfaatkan fungsi GL_LINE_STRIP, GL_LINE_LOOP,
  GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP, GL_QUADS, dan GL_QUAD_STRIP.
*/

/*
Saya menambahkan beberapa fungsi untuk menggambar primitif-primitif tersebut
dengan warna yang berbeda-beda. Fungsi-fungsi tersebut adalah:
  - gambarSpiral(float x, float y)
  - gambarBintang(float x, float y)
  - gambarLingkaran(float x, float y, float radius)
  - gambarSegitigaStrip(float x, float y)
  - gambarPersegi(float x, float y)
  - gambarPita(float x, float y)

Fungsi-fungsi tersebut digunakan untuk menggambar primitif-primitif yang
diminta pada soal. Fungsi-fungsi tersebut dipanggil pada fungsi display
untuk menggambar primitif-primitif tersebut.

Selain itu, saya juga menambahkan animasi rotasi pada beberapa primitif
dengan menggunakan timer. Timer akan memanggil fungsi timer setiap 16 ms
untuk mengupdate sudut rotasi pada primitif-primitif yang dirotasi.
*/

#include <windows.h>
#include <GL/glut.h>
#include <math.h>

// Variabel global
float sudutRotasi = 0.0f; // Untuk animasi rotasi
bool animasiAktif = true; // Status animasi: aktif/pause

// Fungsi untuk menggambar spiral dengan GL_LINE_STRIP
// GL_LINE_STRIP digunakan untuk menggambar garis yang terhubung satu sama lain
// sehingga membentuk spiral
void gambarSpiral(float x, float y)
{
  glPushMatrix();
  glTranslatef(x, y, 0.0f);

  glBegin(GL_LINE_STRIP);
  // Warna spiral
  glColor3f(0.0f, 1.0f, 1.0f);

  // Menggambar spiral
  for (float sudut = 0.0f; sudut < 10.0f * 3.14159f; sudut += 0.1f)
  {
    float radius = 0.01f * sudut;
    float xPos = radius * cos(sudut);
    float yPos = radius * sin(sudut);

    glVertex2f(xPos, yPos);
  }
  glEnd();
  glPopMatrix();
}

// Fungsi untuk menggambar bintang dengan GL_LINE_LOOP
// GL_LINE_LOOP digunakan untuk menggambar garis yang membentuk poligon tertutup
// dengan garis terakhir menghubungkan titik awal dan akhir
void gambarBintang(float x, float y)
{
  glPushMatrix();
  glTranslatef(x, y, 0.0f);
  glRotatef(sudutRotasi, 0.0f, 0.0f, 1.0f);

  glBegin(GL_LINE_LOOP);
  // Warna bintang
  glColor3f(1.0f, 1.0f, 0.0f);

  // Menggambar 5 titik bintang
  for (int i = 0; i < 5; i++)
  {
    float sudut = 2.0f * 3.14159f * i / 5.0f;
    // Titik luar
    glVertex2f(0.2f * sin(sudut), 0.2f * cos(sudut));

    // Titik dalam
    sudut += 3.14159f / 5.0f;
    glVertex2f(0.08f * sin(sudut), 0.08f * cos(sudut));
  }
  glEnd();
  glPopMatrix();
}

// Fungsi untuk menggambar lingkaran dengan GL_TRIANGLE_FAN
// GL_TRIANGLE_FAN digunakan untuk menggambar poligon tertutup dengan titik pusat
// dan titik-titik pada keliling lingkaran
void gambarLingkaran(float x, float y, float radius)
{
  glBegin(GL_TRIANGLE_FAN);
  // Titik pusat lingkaran
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f(x, y);

  // Membuat titik-titik pada keliling lingkaran
  for (int i = 0; i <= 32; i++)
  {
    float sudut = 2.0f * 3.14159f * i / 32;
    float xPos = x + radius * cos(sudut);
    float yPos = y + radius * sin(sudut);

    // Warna sederhana
    glColor3f(1.0f, 0.5f + 0.5f * sin(sudut), 0.5f);
    glVertex2f(xPos, yPos);
  }
  glEnd();
}

// Fungsi untuk menggambar segitiga dengan GL_TRIANGLE_STRIP
// GL_TRIANGLE_STRIP digunakan untuk menggambar poligon tertutup dengan 3 titik
// yang membentuk segitiga
void gambarSegitigaStrip(float x, float y)
{
  glPushMatrix();
  glTranslatef(x, y, 0.0f);

  glBegin(GL_TRIANGLE_STRIP);
  // Segitiga berurutan
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2f(-0.2f, -0.1f);

  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex2f(0.0f, 0.1f);

  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex2f(0.2f, -0.1f);

  glColor3f(1.0f, 1.0f, 0.0f);
  glVertex2f(0.4f, 0.1f);
  glEnd();

  glPopMatrix();
}

// Fungsi untuk menggambar persegi dengan GL_QUADS
// GL_QUADS digunakan untuk menggambar poligon tertutup dengan 4 titik
// yang membentuk persegi
void gambarPersegi(float x, float y)
{
  glPushMatrix();
  glTranslatef(x, y, 0.0f);
  glRotatef(sudutRotasi / 2.0f, 0.0f, 0.0f, 1.0f);

  glBegin(GL_QUADS);
  // Warna untuk setiap titik
  glColor3f(1.0f, 0.0f, 0.0f); // Merah
  glVertex2f(-0.1f, -0.1f);

  glColor3f(0.0f, 1.0f, 0.0f); // Hijau
  glVertex2f(0.1f, -0.1f);

  glColor3f(0.0f, 0.0f, 1.0f); // Biru
  glVertex2f(0.1f, 0.1f);

  glColor3f(1.0f, 1.0f, 0.0f); // Kuning
  glVertex2f(-0.1f, 0.1f);
  glEnd();

  glPopMatrix();
}

// Fungsi untuk menggambar pita dengan GL_QUAD_STRIP
// GL_QUAD_STRIP digunakan untuk menggambar poligon tertutup dengan 4 titik
// yang membentuk pita
void gambarPita(float x, float y)
{
  glPushMatrix();
  glTranslatef(x, y, 0.0f);
  glRotatef(sudutRotasi, 0.0f, 0.0f, 1.0f);

  glBegin(GL_QUAD_STRIP);
  // Menggambar pita sederhana
  for (float t = 0.0f; t <= 6.28f; t += 0.5f)
  {
    // Sisi luar
    glColor3f(0.8f, 0.2f, 0.8f);
    glVertex2f(0.3f * cos(t), 0.3f * sin(t));

    // Sisi dalam
    glColor3f(0.2f, 0.8f, 0.2f);
    glVertex2f(0.2f * cos(t), 0.2f * sin(t));
  }
  glEnd();

  glPopMatrix();
}

// Fungsi display utama
void display()
{
  // Bersihkan layar dengan warna latar belakang
  glClear(GL_COLOR_BUFFER_BIT);

  // Gambar semua bentuk primitif dengan posisi yang lebih terpisah
  gambarLingkaran(-0.6f, 0.6f, 0.2f); // GL_TRIANGLE_FAN (posisi digeser lebih ke pojok)
  gambarBintang(0.6f, 0.6f);          // GL_LINE_LOOP (posisi digeser lebih ke pojok)
  gambarSpiral(-0.6f, -0.6f);         // GL_LINE_STRIP (posisi digeser lebih ke pojok)
  gambarPersegi(0.6f, -0.6f);         // GL_QUADS (posisi digeser lebih ke pojok)

  // Memisahkan dua bentuk di tengah agar tidak tumpang tindih
  gambarSegitigaStrip(0.0f, 0.25f); // GL_TRIANGLE_STRIP (digeser ke atas)
  gambarPita(0.0f, -0.4f);          // GL_QUAD_STRIP (digeser lebih ke bawah)

  glutSwapBuffers();
}

// Fungsi timer untuk animasi
void timer(int value)
{
  if (animasiAktif)
  {
    // Update sudut rotasi untuk animasi
    sudutRotasi += 1.0f;
  }

  glutPostRedisplay();
  glutTimerFunc(16, timer, 0); // ~60 FPS
}

// Fungsi untuk menangani input keyboard
void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27: // Escape = keluar
    exit(0);
    break;
  case 32: // Spasi = pause/resume animasi
    animasiAktif = !animasiAktif;
    break;
  }
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutCreateWindow("Tugas Praktikum 2: Primitif Drawing (Nomor 1)");

  // Warna latar belakang
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  // Daftarkan fungsi-fungsi callback
  glutDisplayFunc(display);
  glutTimerFunc(16, timer, 0);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return 0;
}
