/* Nama File : mobil3D_depth_lighting.cpp
  Deskripsi : Program untuk menampilkan objek 3D (mobil) dengan pencahayaan
              dan depth.
  Pembuat   : Sulhan Fuadi (24060123130115)
  Tanggal   : 25 Maret 2025
*/

/*
Tugas Praktikum 6:
Apabila sebelumnya sudah pernah membuat sebuah mobil 2D, sekarang buatlah
sebuah mobil 3D yang memanfaatkan Depth dan Lighting.
Catatan :
  1. Mobil yang dibangun sebaiknya diberi roda sehingga lebih kreatif.
  2. Agar depth dan Lighting bekerja, mobil yang dibuat harus terbentuk dari
  sebuah rangka. (seperti membuat rangka kubus yang terdiri dari 6 kotak
  kemudian dirotasi dan ditranslasi menjadi kubus)
  3.Sebelumnya sudah diberi template, yang hanya kotak dan roda.
  Buat mobil menjadi lebih bagus lagi, kembangkan lagi lebih baik.
*/

/*
  PANDUAN KONTROL KEYBOARD:
  --------------------------
  Navigasi Mobil:
    w - Bergerak maju (mobil bergerak ke depan sesuai arah hadap)
    s - Bergerak mundur (mobil bergerak ke belakang sesuai arah hadap)
    a - Belok ke kiri (mobil berputar berlawanan arah jarum jam)
    d - Belok ke kanan (mobil berputar searah jarum jam)

  Kontrol Tambahan:
    r - Reset posisi mobil (mengembalikan mobil ke posisi awal di tengah layar)
    Esc - Keluar dari aplikasi (mengakhiri program)

  Catatan: Kecepatan mobil dan sudut belok telah diatur untuk memberikan
  pengalaman kendali yang nyaman. Arah pergerakan mobil selalu relatif
  terhadap orientasi mobil saat ini.
*/

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

void Mobil()
{
  // Membuat badan utama mobil (bagian tengah)
  glPushMatrix();
  glColor3f(0.0f, 0.6f, 0.0f);    // Warna hijau untuk badan mobil
  glTranslatef(0.0f, 1.0f, 0.0f); // Posisi di tengah
  glScalef(4.0f, 1.0f, 2.0f);     // Skala ukuran badan mobil
  glutSolidCube(1.0);             // Membuat kubus untuk badan utama
  glPopMatrix();

  // Membuat bagian kap mobil (depan)
  glPushMatrix();
  glColor3f(0.0f, 0.7f, 0.0f);    // Warna hijau untuk kap mobil
  glTranslatef(2.5f, 1.0f, 0.0f); // Posisi di depan badan utama
  glScalef(1.0f, 1.0f, 2.0f);     // Skala ukuran kap mobil
  glutSolidCube(1.0);
  glPopMatrix();

  // Membuat bagian belakang mobil (bagasi)
  glPushMatrix();
  glColor3f(0.0f, 0.7f, 0.0f); // Warna hijau untuk bagian belakang
  glTranslatef(-2.5f, 1.0f, 0.0f);
  glScalef(1.0f, 1.0f, 2.0f);
  glutSolidCube(1.0);
  glPopMatrix();

  // Membuat atap mobil (kabin)
  glPushMatrix();
  glColor3f(0.0f, 0.5f, 0.0f);    // Warna hijau tua untuk atap
  glTranslatef(0.0f, 2.0f, 0.0f); // Posisi di atas badan utama
  glScalef(3.0f, 1.0f, 2.0f);
  glutSolidCube(1.0);
  glPopMatrix();

  // Membuat kaca depan (windshield) dengan sudut miring
  glPushMatrix();
  glColor3f(0.4f, 0.8f, 1.0f); // Warna biru muda untuk kaca
  glTranslatef(1.5f, 1.5f, 0.0f);
  glRotatef(45.0f, 0.0f, 0.0f, 1.0f); // Rotasi untuk membuat kaca miring
  glScalef(0.1f, 1.4f, 1.9f);
  glutSolidCube(1.0);
  glPopMatrix();

  // Membuat kaca belakang dengan sudut miring
  glPushMatrix();
  glColor3f(0.4f, 0.8f, 1.0f);
  glTranslatef(-1.5f, 1.5f, 0.0f);
  glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // Rotasi negatif untuk kaca belakang
  glScalef(0.1f, 1.4f, 1.9f);
  glutSolidCube(1.0);
  glPopMatrix();

  // Membuat lampu depan kanan
  glPushMatrix();
  glColor3f(1.0f, 1.0f, 0.0f); // Warna kuning untuk lampu depan
  glTranslatef(3.0f, 1.0f, 0.7f);
  glScalef(0.1f, 0.3f, 0.3f);
  glutSolidCube(1.0);
  glPopMatrix();

  // Membuat lampu depan kiri
  glPushMatrix();
  glColor3f(1.0f, 1.0f, 0.0f);
  glTranslatef(3.0f, 1.0f, -0.7f);
  glScalef(0.1f, 0.3f, 0.3f);
  glutSolidCube(1.0);
  glPopMatrix();

  // Membuat lampu belakang kanan
  glPushMatrix();
  glColor3f(1.0f, 0.0f, 0.0f); // Warna merah untuk lampu belakang
  glTranslatef(-3.0f, 1.0f, 0.7f);
  glScalef(0.1f, 0.3f, 0.3f);
  glutSolidCube(1.0);
  glPopMatrix();

  // Membuat lampu belakang kiri
  glPushMatrix();
  glColor3f(1.0f, 0.0f, 0.0f);
  glTranslatef(-3.0f, 1.0f, -0.7f);
  glScalef(0.1f, 0.3f, 0.3f);
  glutSolidCube(1.0);
  glPopMatrix();

  // Membuat roda depan kanan dengan ban dan velg
  glPushMatrix();
  glColor3f(0.2f, 0.2f, 0.2f); // Warna hitam untuk ban
  glTranslatef(2.0f, 0.5f, 1.0f);
  glRotatef(0.0f, 1.0f, 0.0f, 0.0f); // Hapus rotasi untuk membuat roda vertikal
  glutSolidTorus(0.2, 0.4, 10, 16);  // Ban mobil berbentuk torus
  glPopMatrix();

  // Membuat velg roda depan kanan
  glPushMatrix();
  glColor3f(0.8f, 0.8f, 0.8f); // Warna abu-abu terang untuk velg
  glTranslatef(2.0f, 0.5f, 1.0f);
  glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
  glutSolidTorus(0.05, 0.3, 10, 16); // Velg lebih kecil dari ban
  glPopMatrix();

  // Membuat roda depan kiri dengan ban dan velg
  glPushMatrix();
  glColor3f(0.2f, 0.2f, 0.2f);
  glTranslatef(2.0f, 0.5f, -1.0f);
  glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
  glutSolidTorus(0.2, 0.4, 10, 16);
  glPopMatrix();

  // Membuat velg roda depan kiri
  glPushMatrix();
  glColor3f(0.8f, 0.8f, 0.8f);
  glTranslatef(2.0f, 0.5f, -1.0f);
  glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
  glutSolidTorus(0.05, 0.3, 10, 16);
  glPopMatrix();

  // Membuat roda belakang kanan dengan ban dan velg
  glPushMatrix();
  glColor3f(0.2f, 0.2f, 0.2f);
  glTranslatef(-2.0f, 0.5f, 1.0f);
  glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
  glutSolidTorus(0.2, 0.4, 10, 16);
  glPopMatrix();

  // Membuat velg roda belakang kanan
  glPushMatrix();
  glColor3f(0.8f, 0.8f, 0.8f);
  glTranslatef(-2.0f, 0.5f, 1.0f);
  glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
  glutSolidTorus(0.05, 0.3, 10, 16);
  glPopMatrix();

  // Membuat roda belakang kiri dengan ban dan velg
  glPushMatrix();
  glColor3f(0.2f, 0.2f, 0.2f);
  glTranslatef(-2.0f, 0.5f, -1.0f);
  glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
  glutSolidTorus(0.2, 0.4, 10, 16);
  glPopMatrix();

  // Membuat velg roda belakang kiri
  glPushMatrix();
  glColor3f(0.8f, 0.8f, 0.8f);
  glTranslatef(-2.0f, 0.5f, -1.0f);
  glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
  glutSolidTorus(0.05, 0.3, 10, 16);
  glPopMatrix();

  // Membuat bumper depan
  glPushMatrix();
  glColor3f(0.1f, 0.1f, 0.1f); // Warna hitam untuk bumper
  glTranslatef(3.0f, 0.6f, 0.0f);
  glScalef(0.2f, 0.3f, 2.2f);
  glutSolidCube(1.0);
  glPopMatrix();

  // Membuat bumper belakang
  glPushMatrix();
  glColor3f(0.1f, 0.1f, 0.1f);
  glTranslatef(-3.0f, 0.6f, 0.0f);
  glScalef(0.2f, 0.3f, 2.2f);
  glutSolidCube(1.0);
  glPopMatrix();

  // Membuat knalpot
  glPushMatrix();
  glColor3f(0.3f, 0.3f, 0.3f);        // Warna abu-abu gelap untuk knalpot
  glTranslatef(-3.2f, 0.5f, -0.7f);   // Posisi di belakang mobil sedikit ke kiri
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotasi agar silinder menghadap belakang

  // Create a quadric object
  GLUquadricObj *cylinder = gluNewQuadric();
  gluQuadricDrawStyle(cylinder, GLU_FILL);
  gluCylinder(cylinder, 0.1, 0.1, 0.4, 10, 1); // Parameters: quadric, base radius, top radius, height, slices, stacks
  gluDeleteQuadric(cylinder);

  glPopMatrix();
}

// Add this at the end of cook.cpp
void init()
{
  // Set background color
  glClearColor(0.7f, 0.7f, 1.0f, 1.0f); // Light blue sky color

  // Enable 3D depth testing
  glEnable(GL_DEPTH_TEST);

  // Enable lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // Configure light 0
  GLfloat light_position[] = {10.0f, 10.0f, 10.0f, 1.0f};
  GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
  GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

  // Enable material properties
  glEnable(GL_COLOR_MATERIAL);
}

// Variabel global untuk posisi dan rotasi mobil
float carX = 0.0f;        // Posisi X mobil
float carY = 0.0f;        // Posisi Y mobil
float carZ = 0.0f;        // Posisi Z mobil
float carRotation = 0.0f; // Rotasi mobil (dalam derajat)

// Fungsi untuk menangani input keyboard
void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'w': // Maju
    carX += sin(carRotation * 3.14159 / 180.0) * 0.5;
    carZ -= cos(carRotation * 3.14159 / 180.0) * 0.5;
    break;
  case 's': // Mundur
    carX -= sin(carRotation * 3.14159 / 180.0) * 0.5;
    carZ += cos(carRotation * 3.14159 / 180.0) * 0.5;
    break;
  case 'a': // Belok kiri
    carRotation += 5.0;
    break;
  case 'd': // Belok kanan
    carRotation -= 5.0;
    break;
  case 'r': // Reset posisi
    carX = 0.0f;
    carY = 0.0f;
    carZ = 0.0f;
    carRotation = 0.0f;
    break;
  case 27: // Esc untuk keluar
    exit(0);
    break;
  }

  glutPostRedisplay(); // Meminta GLUT untuk menggambar ulang layar
}

// Fungsi display perlu diubah
void display()
{
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset transformations
  glLoadIdentity();

  // Set the camera position
  gluLookAt(
      0.0f, 5.0f, 15.0f, // Eye position
      0.0f, 0.0f, 0.0f,  // Look at position
      0.0f, 1.0f, 0.0f   // Up vector
  );

  // Terapkan transformasi pada mobil
  glPushMatrix();
  glTranslatef(carX, carY, carZ);           // Posisikan mobil
  glRotatef(carRotation, 0.0f, 1.0f, 0.0f); // Rotasi mobil pada sumbu Y
  Mobil();                                  // Gambar mobil
  glPopMatrix();

  // Swap buffers (double buffering)
  glutSwapBuffers();
}

void reshape(int width, int height)
{
  // Prevent division by zero
  if (height == 0)
    height = 1;

  // Set viewport to window dimensions
  glViewport(0, 0, width, height);

  // Set projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);

  // Switch back to modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char **argv)
{
  // Initialize GLUT
  glutInit(&argc, argv);

  // Set display mode (double buffer, RGB with depth buffer)
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Set window size and position
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);

  // Create window with a title
  glutCreateWindow("3D Car Model");

  // Register callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  // Initialize OpenGL settings
  init();

  // Enter the GLUT main loop
  glutMainLoop();

  return 0;
}
