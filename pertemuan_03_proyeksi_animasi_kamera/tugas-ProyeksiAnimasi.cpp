/* Nama File : tugas-ProyeksiAnimasi.cpp
  Deskripsi : Program animasi lengan dan jari tangan yang dapat digerakkan dengan keyboard
  Pembuat   : Sulhan Fuadi (24060123130115)
  Tanggal   : 12 Maret 2025
*/

/*
Detail Tugas Praktikum 4 Proyeksi dan Animasi:
  Tambahkanlah telapak tangan beserta jari-jari dari code lengan_bergerak.cpp. Telapak tangan dan
  jari-jari tersebut bisa digerakan menggunakan keyboard!
*/

#include <GL/glut.h>
#include <stdio.h>

// Sudut-sudut sendi utama
static int shoulder = 0, elbow = 0, telapaktangan = 0;

// Sudut-sudut ruas jari pertama
static int jempol_ruas1 = 0, telunjuk_ruas1 = 0, jateng_ruas1 = 0;
static int manis_ruas1 = 0, keling_ruas1 = 0;

// Sudut-sudut ruas jari kedua
static int jempol_ruas2 = 0, telunjuk_ruas2 = 0, jateng_ruas2 = 0;
static int manis_ruas2 = 0, keling_ruas2 = 0;

// Flag untuk menampilkan menu bantuan
static int showHelp = 0;

// Warna-warna untuk bagian tubuh
GLfloat lenganAtas[3] = {0.8, 0.6, 0.5};    // Cokelat muda
GLfloat lenganBawah[3] = {0.7, 0.5, 0.4};   // Cokelat sedang
GLfloat telapakTangan[3] = {0.9, 0.7, 0.6}; // Cokelat sangat muda
GLfloat jariTangan[3] = {0.85, 0.65, 0.55}; // Cokelat agak muda

void init(void)
{
  // Inisialisasi warna latar dan model shading
  glClearColor(0.2, 0.2, 0.3, 0.0); // Warna latar biru gelap biar cakep
  glShadeModel(GL_FLAT);

  // Mengaktifkan pencahayaan sederhana
  glEnable(GL_DEPTH_TEST);
}

// Fungsi untuk menampilkan menu bantuan
void displayHelp()
{
  glColor3f(1.0, 1.0, 1.0); // Warna teks putih
  glRasterPos3f(-4.0, 3.5, 0.0);

  char *helpText[] = {
      "Kontrol Keyboard:",
      "s/S - Putar bahu kanan/kiri",
      "e/E - Tekuk/Luruskan siku",
      "0/) - Putar pergelangan tangan",
      "1/! - Tekuk/Luruskan ruas kedua jempol",
      "2/@ - Tekuk/Luruskan ruas pertama telunjuk",
      "q/Q - Tekuk/Luruskan ruas kedua telunjuk",
      "3/# - Tekuk/Luruskan ruas pertama jari tengah",
      "w/W - Tekuk/Luruskan ruas kedua jari tengah",
      "4/$ - Tekuk/Luruskan ruas pertama jari manis",
      "r/R - Tekuk/Luruskan ruas kedua jari manis",
      "5/% - Tekuk/Luruskan ruas pertama kelingking",
      "t/T - Tekuk/Luruskan ruas kedua kelingking",
      "6/^ - Tekuk/Luruskan ruas pertama jempol",
      "h - Tampilkan/Sembunyikan bantuan",
      "ESC - Keluar"};

  // Menampilkan setiap baris teks bantuan
  for (int i = 0; i < 16; i++)
  {
    glRasterPos3f(-4.0, 3.5 - (i * 0.3), 0.0);
    for (char *c = helpText[i]; *c != '\0'; c++)
    {
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
  }
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Menampilkan menu bantuan jika diperlukan
  if (showHelp)
  {
    displayHelp();
  }

  glPushMatrix();

  // Posisikan lengan
  glTranslatef(-1.0, 0.0, 0.0);

  // Bagian lengan atas (bahu ke siku)
  glRotatef((GLfloat)shoulder, 0.0, 1.0, 0.0); // Rotasi pada sumbu y untuk gerakan bahu
  glTranslatef(1.0, 0.0, 0.0);
  glPushMatrix();
  glColor3fv(lenganAtas); // Warna lengan atas
  glScalef(2.0, 0.4, 0.6);
  glutWireCube(1.0);
  glPopMatrix();

  // Bagian lengan bawah (siku ke pergelangan)
  glTranslatef(1.0, 0.0, 0.0);
  glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0); // Rotasi pada sumbu z untuk gerakan siku
  glTranslatef(1.0, 0.0, 0.0);
  glPushMatrix();
  glColor3fv(lenganBawah); // Warna lengan bawah
  glScalef(2.0, 0.4, 0.6);
  glutWireCube(1.0);
  glPopMatrix();

  // Bagian telapak tangan
  glTranslatef(1.0, 0.0, 0.0);
  glRotatef((GLfloat)telapaktangan, 1.0, 0.0, 0.0); // Rotasi pada sumbu x untuk gerakan pergelangan
  glTranslatef(0.2, 0.0, 0.0);
  glPushMatrix();
  glColor3fv(telapakTangan); // Warna telapak tangan
  glScalef(0.5, 0.2, 1.0);
  glutWireCube(1.0);
  glPopMatrix();

  // Jempol - Ruas pertama
  glColor3fv(jariTangan); // Warna jari tangan
  glPushMatrix();
  glTranslatef(-0.05, 0.0, 0.35);
  glRotatef((GLfloat)jempol_ruas1, 1.0, 0.0, 0.0); // Rotasi jempol ruas 1
  glTranslatef(0.0, 0.0, 0.3);
  glScalef(0.2, 0.2, 0.3);
  glutWireCube(1.0);

  // Jempol - Ruas kedua
  glTranslatef(0.0, 0.0, 1.0);
  glRotatef((GLfloat)jempol_ruas2, 1.0, 0.0, 0.0); // Rotasi jempol ruas 2
  glTranslatef(0.0, 0.0, 0.5);
  glScalef(1.0, 1.0, 0.7);
  glutWireCube(1.0);
  glPopMatrix();

  // Telunjuk - Ruas pertama
  glPushMatrix();
  glTranslatef(0.35, 0.0, 0.35);
  glRotatef((GLfloat)telunjuk_ruas1, 0.0, 0.0, 1.0); // Rotasi telunjuk ruas 1
  glTranslatef(0.3, 0.0, 0.0);
  glScalef(0.6, 0.2, 0.15);
  glutWireCube(1.0);

  // Telunjuk - Ruas kedua
  glTranslatef(1.0, 0.0, 0.0);
  glRotatef((GLfloat)telunjuk_ruas2, 0.0, 0.0, 1.0); // Rotasi telunjuk ruas 2
  glTranslatef(0.5, 0.0, 0.0);
  glScalef(1.0, 1.0, 1.0);
  glutWireCube(1.0);
  glPopMatrix();

  // Jari tengah - Ruas pertama
  glPushMatrix();
  glTranslatef(0.35, 0.0, 0.125);
  glRotatef((GLfloat)jateng_ruas1, 0.0, 0.0, 1.0); // Rotasi jari tengah ruas 1
  glTranslatef(0.375, 0.0, 0.0);
  glScalef(0.75, 0.2, 0.15);
  glutWireCube(1.0);

  // Jari tengah - Ruas kedua
  glTranslatef(1.0, 0.0, 0.0);
  glRotatef((GLfloat)jateng_ruas2, 0.0, 0.0, 1.0); // Rotasi jari tengah ruas 2
  glTranslatef(0.5, 0.0, 0.0);
  glScalef(1.0, 1.0, 1.0);
  glutWireCube(1.0);
  glPopMatrix();

  // Jari manis - Ruas pertama
  glPushMatrix();
  glTranslatef(0.35, 0.0, -0.1);
  glRotatef((GLfloat)manis_ruas1, 0.0, 0.0, 1.0); // Rotasi jari manis ruas 1
  glTranslatef(0.3, 0.0, 0.0);
  glScalef(0.6, 0.2, 0.15);
  glutWireCube(1.0);

  // Jari manis - Ruas kedua
  glTranslatef(1.0, 0.0, 0.0);
  glRotatef((GLfloat)manis_ruas2, 0.0, 0.0, 1.0); // Rotasi jari manis ruas 2
  glTranslatef(0.5, 0.0, 0.0);
  glScalef(1.0, 1.0, 1.0);
  glutWireCube(1.0);
  glPopMatrix();

  // Kelingking - Ruas pertama
  glPushMatrix();
  glTranslatef(0.35, 0.0, -0.325);
  glRotatef((GLfloat)keling_ruas1, 0.0, 0.0, 1.0); // Rotasi kelingking ruas 1
  glTranslatef(0.25, 0.0, 0.0);
  glScalef(0.5, 0.2, 0.15);
  glutWireCube(1.0);

  // Kelingking - Ruas kedua
  glTranslatef(1.0, 0.0, 0.0);
  glRotatef((GLfloat)keling_ruas2, 0.0, 0.0, 1.0); // Rotasi kelingking ruas 2
  glTranslatef(0.5, 0.0, 0.0);
  glScalef(1.0, 1.0, 1.0);
  glutWireCube(1.0);
  glPopMatrix();

  glPopMatrix();
  glutSwapBuffers();
}

// Fungsi untuk mengatur proyeksi dan perspektif pandangan
void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -5.0);
}

// Fungsi untuk mengatur kontrol keyboard
void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  // Kontrol untuk bahu
  case 's': // Putar bahu ke kanan
    shoulder = (shoulder + 5) % 360;
    glutPostRedisplay();
    break;

  case 'S': // Putar bahu ke kiri
    shoulder = (shoulder - 5) % 360;
    glutPostRedisplay();
    break;

  // Kontrol untuk siku
  case 'e':          // Tekuk siku
    if (elbow < 150) // Batasi gerakan maksimum
    {
      elbow = (elbow + 5);
    }
    glutPostRedisplay();
    break;

  case 'E':        // Luruskan siku
    if (elbow > 0) // Batasi gerakan minimum
    {
      elbow = (elbow - 5);
    }
    glutPostRedisplay();
    break;

  // Kontrol untuk pergelangan tangan
  case '0':                 // Putar telapak tangan ke atas
    if (telapaktangan < 90) // Batasi gerakan ke atas
    {
      telapaktangan = (telapaktangan + 5);
    }
    glutPostRedisplay();
    break;

  case ')':                  // Putar telapak tangan ke bawah
    if (telapaktangan > -90) // Batasi gerakan ke bawah
    {
      telapaktangan = (telapaktangan - 5);
    }
    glutPostRedisplay();
    break;

  // Kontrol untuk jari telunjuk
  case '2': // Tekuk ruas pertama jari telunjuk
    if (telunjuk_ruas1 < 90)
    {
      telunjuk_ruas1 = (telunjuk_ruas1 + 5);
    }
    glutPostRedisplay();
    break;

  case '@': // Luruskan ruas pertama jari telunjuk
    if (telunjuk_ruas1 > 0)
    {
      telunjuk_ruas1 = (telunjuk_ruas1 - 5);
    }
    glutPostRedisplay();
    break;

  case 'q': // Tekuk ruas kedua jari telunjuk
    if (telunjuk_ruas2 < 90)
    {
      telunjuk_ruas2 = (telunjuk_ruas2 + 5);
    }
    glutPostRedisplay();
    break;

  case 'Q': // Luruskan ruas kedua jari telunjuk
    if (telunjuk_ruas2 > 0)
    {
      telunjuk_ruas2 = (telunjuk_ruas2 - 5);
    }
    glutPostRedisplay();
    break;

  // Kontrol untuk jempol
  case '6': // Tekuk ruas pertama jempol
    if (jempol_ruas1 < 90)
    {
      jempol_ruas1 = (jempol_ruas1 + 5);
    }
    glutPostRedisplay();
    break;

  case '^': // Luruskan ruas pertama jempol
    if (jempol_ruas1 > 0)
    {
      jempol_ruas1 = (jempol_ruas1 - 5);
    }
    glutPostRedisplay();
    break;

  case '1': // Tekuk ruas kedua jempol
    if (jempol_ruas2 < 90)
    {
      jempol_ruas2 = (jempol_ruas2 + 5);
    }
    glutPostRedisplay();
    break;

  case '!': // Luruskan ruas kedua jempol
    if (jempol_ruas2 > 0)
    {
      jempol_ruas2 = (jempol_ruas2 - 5);
    }
    glutPostRedisplay();
    break;

  // Kontrol untuk jari tengah
  case '3': // Tekuk ruas pertama jari tengah
    if (jateng_ruas1 < 90)
    {
      jateng_ruas1 = (jateng_ruas1 + 5);
    }
    glutPostRedisplay();
    break;

  case '#': // Luruskan ruas pertama jari tengah
    if (jateng_ruas1 > 0)
    {
      jateng_ruas1 = (jateng_ruas1 - 5);
    }
    glutPostRedisplay();
    break;

  case 'w': // Tekuk ruas kedua jari tengah
    if (jateng_ruas2 < 90)
    {
      jateng_ruas2 = (jateng_ruas2 + 5);
    }
    glutPostRedisplay();
    break;

  case 'W': // Luruskan ruas kedua jari tengah
    if (jateng_ruas2 > 0)
    {
      jateng_ruas2 = (jateng_ruas2 - 5);
    }
    glutPostRedisplay();
    break;

  // Kontrol untuk jari manis
  case '4': // Tekuk ruas pertama jari manis
    if (manis_ruas1 < 90)
    {
      manis_ruas1 = (manis_ruas1 + 5);
    }
    glutPostRedisplay();
    break;

  case '$': // Luruskan ruas pertama jari manis
    if (manis_ruas1 > 0)
    {
      manis_ruas1 = (manis_ruas1 - 5);
    }
    glutPostRedisplay();
    break;

  case 'r': // Tekuk ruas kedua jari manis
    if (manis_ruas2 < 90)
    {
      manis_ruas2 = (manis_ruas2 + 5);
    }
    glutPostRedisplay();
    break;

  case 'R': // Luruskan ruas kedua jari manis
    if (manis_ruas2 > 0)
    {
      manis_ruas2 = (manis_ruas2 - 5);
    }
    glutPostRedisplay();
    break;

  // Kontrol untuk kelingking
  case '5': // Tekuk ruas pertama kelingking
    if (keling_ruas1 < 90)
    {
      keling_ruas1 = (keling_ruas1 + 5);
    }
    glutPostRedisplay();
    break;

  case '%': // Luruskan ruas pertama kelingking
    if (keling_ruas1 > 0)
    {
      keling_ruas1 = (keling_ruas1 - 5);
    }
    glutPostRedisplay();
    break;

  case 't': // Tekuk ruas kedua kelingking
    if (keling_ruas2 < 90)
    {
      keling_ruas2 = (keling_ruas2 + 5);
    }
    glutPostRedisplay();
    break;

  case 'T': // Luruskan ruas kedua kelingking
    if (keling_ruas2 > 0)
    {
      keling_ruas2 = (keling_ruas2 - 5);
    }
    glutPostRedisplay();
    break;

  // Tampilkan atau sembunyikan menu bantuan
  case 'h':
    showHelp = !showHelp;
    glutPostRedisplay();
    break;

  // Keluar dari program
  case 27: // Tombol ESC
    exit(0);
    break;

  default:
    break;
  }
}

// Fungsi utama
int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1000, 900);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Tugas Praktikum 4: Proyeksi dan Animasi (Animasi Lengan, Telapak Tangan, dan Jari)");
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  // Tampilkan instruksi penggunaan di konsol
  printf("=== PROGRAM ANIMASI LENGAN DAN JARI ===\n");
  printf("Tekan 'h' untuk menampilkan bantuan kontrol keyboard\n");
  printf("Tekan ESC untuk keluar\n");

  glutMainLoop();
  return 0;
}
