/* Nama File : nomor2-PrimitifDrawing.cpp
  Deskripsi : Program untuk menampilkan kubus bertingkat dengan warna gradien
  Pembuat   : Sulhan Fuadi (24060123130115)
  Tanggal   : 09 Maret 2025
*/

/*
Tugas Praktikum 2 nomor 2:
  Buat Gambar Kubus (Persegi 2D) bertingkat dari objek primitif sekreatif mungkin (minimal 5 kubus).
*/

/*
Saya membuat program yang menampilkan susunan kubus bertingkat dengan warna gradien.
Program ini menggunakan beberapa fungsi untuk menggambar persegi dengan warna gradien
dari kiri atas ke kanan bawah dan dari tengah ke luar. Fungsi-fungsi tersebut adalah:
  - persegiBerGradien(float x, float y, float size, float r1, float g1, float b1, float r2, float g2, float b2)
  - persegiGradienRadial(float x, float y, float size, float r1, float g1, float b1, float r2, float g2, float b2)

Fungsi-fungsi tersebut digunakan untuk menggambar kubus bertingkat dengan warna gradien
dari kiri atas ke kanan bawah dan dari tengah ke luar. Fungsi-fungsi tersebut dipanggil
pada fungsi display untuk menggambar kubus bertingkat tersebut.

Selain itu, saya juga menambahkan efek animasi perubahan warna pada beberapa kubus
dengan menggunakan waktu sebagai variabel untuk mengubah warna secara dinamis.
*/

#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

// Fungsi untuk membuat persegi dengan warna gradien
void persegiBerGradien(float x, float y, float size,
                       float r1, float g1, float b1,
                       float r2, float g2, float b2)
{
  // Membuat persegi dengan warna gradien dari kiri atas ke kanan bawah
  glBegin(GL_TRIANGLE_STRIP);
  // Titik kiri atas
  glColor3f(r1, g1, b1);
  glVertex2f(x, y + size);

  // Titik kiri bawah
  glColor3f(r1, g1, b1);
  glVertex2f(x, y);

  // Titik kanan atas
  glColor3f(r2, g2, b2);
  glVertex2f(x + size, y + size);

  // Titik kanan bawah
  glColor3f(r2, g2, b2);
  glVertex2f(x + size, y);
  glEnd();
}

// Fungsi untuk membuat persegi dengan warna gradien radial
void persegiGradienRadial(float x, float y, float size,
                          float r1, float g1, float b1,
                          float r2, float g2, float b2)
{
  // Mendapatkan titik tengah persegi
  float centerX = x + size / 2.0f;
  float centerY = y + size / 2.0f;

  // Membuat persegi dengan warna gradien dari tengah ke luar
  glBegin(GL_TRIANGLE_FAN);
  // Titik tengah
  glColor3f(r1, g1, b1);
  glVertex2f(centerX, centerY);

  // Titik sudut
  glColor3f(r2, g2, b2);
  glVertex2f(x, y);               // kiri bawah
  glVertex2f(x, y + size);        // kiri atas
  glVertex2f(x + size, y + size); // kanan atas
  glVertex2f(x + size, y);        // kanan bawah
  glVertex2f(x, y);               // kembali ke kiri bawah untuk menutup fan
  glEnd();
}

// Fungsi untuk animasi perubahan warna berdasarkan waktu
float getColorPulse(float intensity)
{
  // Menggunakan fungsi sinus untuk mendapatkan nilai yang berubah-ubah
  return (sin(glutGet(GLUT_ELAPSED_TIME) * 0.001 * intensity) + 1.0f) / 2.0f;
}

// Fungsi untuk menggambar bingkai persegi
void persegiBingkai(float x, float y, float size)
{
  glBegin(GL_LINE_LOOP);
  glVertex2f(x, y);
  glVertex2f(x, y + size);
  glVertex2f(x + size, y + size);
  glVertex2f(x + size, y);
  glEnd();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  // Definisikan ukuran persegi yang konsisten
  const float SIZE = 0.2f;

  // Mendapatkan nilai warna dinamis untuk efek animasi
  float pulseRed = getColorPulse(1.5f);
  float pulseGreen = getColorPulse(2.0f);
  float pulseBlue = getColorPulse(2.5f);

  // Layer 1 (dasar) - 5 persegi berjajar
  // Posisi y untuk layer 1
  float y1 = -0.7f;

  // Persegi paling kiri
  persegiBerGradien(-0.5f, y1, SIZE, 0.8f, 0.2f, 0.2f, 1.0f, 0.8f, 0.2f);

  // Persegi kiri
  persegiBerGradien(-0.3f, y1, SIZE, 0.8f, 0.2f, 0.2f, 1.0f, 0.8f, 0.2f);

  // Persegi tengah
  persegiGradienRadial(-0.1f, y1, SIZE, 0.2f, 0.8f, 0.2f, 0.1f, 0.4f, 0.1f);

  // Persegi kanan
  persegiBerGradien(0.1f, y1, SIZE, 0.2f, 0.2f, 0.8f, 0.8f, 0.2f, 0.8f);

  // Persegi paling kanan
  persegiBerGradien(0.3f, y1, SIZE, 0.2f, 0.2f, 0.8f, 0.8f, 0.2f, 0.8f);

  // Layer 2 - 3 persegi berjajar
  float y2 = -0.5f;

  // Persegi kiri
  persegiBerGradien(-0.3f, y2, SIZE, 0.0f, 0.7f, 0.7f, 0.2f, 0.8f, 0.2f);

  // Persegi tengah
  persegiBerGradien(-0.1f, y2, SIZE, 1.0f, 1.0f, 0.0f, 1.0f, 0.6f, 0.0f);

  // Persegi kanan
  persegiBerGradien(0.1f, y2, SIZE, 0.0f, 0.7f, 0.7f, 0.2f, 0.8f, 0.2f);

  // Layer 3 - 2 persegi
  float y3 = -0.3f;

  // Persegi kiri
  persegiGradienRadial(-0.2f, y3, SIZE, pulseRed, pulseGreen, pulseBlue, 0.3f, 0.3f, 0.7f);

  // Persegi kanan
  persegiGradienRadial(0.0f, y3, SIZE, pulseBlue, pulseRed, pulseGreen, 0.7f, 0.3f, 0.3f);

  // Layer 4 - 1 persegi
  float y4 = -0.1f;
  persegiBerGradien(-0.1f, y4, SIZE, 0.8f, 0.2f, 0.2f, 0.2f, 0.2f, 0.8f);

  // Layer 5 (paling atas) - 1 persegi
  float y5 = 0.1f;
  persegiGradienRadial(-0.1f, y5, SIZE, 1.0f, 0.8f, 0.0f, 0.8f, 0.4f, 0.0f);

  // Bingkai persegi (garis putih)
  glLineWidth(1.5f);
  glColor3f(1.0f, 1.0f, 1.0f);

  // Bingkai untuk semua persegi - Layer 1
  persegiBingkai(-0.5f, y1, SIZE);
  persegiBingkai(-0.3f, y1, SIZE);
  persegiBingkai(-0.1f, y1, SIZE);
  persegiBingkai(0.1f, y1, SIZE);
  persegiBingkai(0.3f, y1, SIZE);

  // Bingkai untuk Layer 2
  persegiBingkai(-0.3f, y2, SIZE);
  persegiBingkai(-0.1f, y2, SIZE);
  persegiBingkai(0.1f, y2, SIZE);

  // Bingkai untuk Layer 3
  persegiBingkai(-0.2f, y3, SIZE);
  persegiBingkai(0.0f, y3, SIZE);

  // Bingkai untuk Layer 4
  persegiBingkai(-0.1f, y4, SIZE);

  // Bingkai untuk Layer 5
  persegiBingkai(-0.1f, y5, SIZE);

  glutSwapBuffers();
}

// Fungsi untuk mengatur perubahan ukuran jendela
void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
}

// Fungsi untuk mendukung animasi
void timer(int value)
{
  glutPostRedisplay();
  glutTimerFunc(16, timer, 0); // ~60 FPS
}

int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(800, 800);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutCreateWindow("Tugas Praktikum 2: Primitif Drawing (Nomor 2)");

  // Mengatur warna latar belakang hitam untuk kontras lebih baik
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Mendaftarkan fungsi callback
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutTimerFunc(0, timer, 0);

  glutMainLoop();
  return 0;
}