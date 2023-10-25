#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define STEP 3
#define ANGULO 15
// float alfa = (ANGULO * (3,141592653589/180alfa
GLfloat angle, fAspect;

// [x,y,z]camera, [x,y,z]alvo, [x,y,z]vetor up
int cam[3][3] = { {-200, 5, 0},
				  {0, 0, 0},
				  {0, 1, 0}
};

//left, right, bottom, top, near, far
int ort[3][2] = { {-260, 260},
				  {-84, 145},
				  {-400, 400}
};

//variáveis para controle de rotação do conjunto de objetos
int anguloX = 0;
int anguloY = 0;

//Flags
bool isOrtho = false;

void DesenhaMesa(void){
	glColor3f(0.2, 0.0, 0.0);
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-200.0, 0.0, 200.0);
		glVertex3f(200.0, 0.0, 200.0);
		glVertex3f(200.0, 0.0, -200.0);
		glVertex3f(-200.0, 0.0, -200.0);
	glEnd();
}

void DesenhaTorus(void){
	glColor3f(1.0, 0.5, 1.0);
	glPushMatrix();
		glTranslatef(50.0, 0.0, 50);
		glRotatef(90, 1, 0, 0);
		glutSolidTorus(5, 10, 20, 20); //(raio interno, raio externo, aros diametro, aros espessura)
	glPopMatrix();

	glColor3f(0.0, 0.5, 0.5);
	glPushMatrix();
		glTranslatef(-50.0, 0.0, -50);
		glRotatef(90, 1, 0, 0);
		glutSolidTorus(5, 10, 20, 20); //(raio interno, raio externo, aros diametro, aros espessura)
	glPopMatrix();
}

void DesenhaBules(void){
	glColor3f(0.0f, 0.0f, 1.0f);
	glPushMatrix();
		glTranslatef(50.0, 0.0, -50.0);
		glRotatef(225, 0, 1, 0);
		glutSolidTeapot(10.0f);
	glPopMatrix();

	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
		glTranslatef(-50.0, 0.0, 50.0);
		glRotatef(45, 0, 1, 0);
		glutSolidTeapot(10.0f);
	glPopMatrix();
}

// Função callback chamada para fazer o desenho
void Desenha(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glRotatef(anguloX, 1, 0, 0);
	glRotatef(anguloY, 0, 1, 0);

	DesenhaMesa();
	DesenhaBules();
	DesenhaTorus();

	// Executa os comandos OpenGL
	glutSwapBuffers();
 }

// Inicializa parâmetros de rendering
void Inicializa (void){ 
	GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0}; 
	GLfloat luzDifusa[4]={0.7, 0.7, 0.7, 1.0};		// "cor" 
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};	// "brilho" da luz [R, G, B, ALPHA(transparencia)]
	GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0, 1.0, 1.0, 1.0};	//cor que o objeto vai refletir [R, G, B, ALPHA(transparencia)]
	GLint especMaterial = 60;

 	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	// Habilita o modelo de colorização de Gouraud (liso ou low poly)
	glShadeModel(GL_SMOOTH);

	// Define a refletância do material 
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);  
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	
	angle=45;
}    


// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();



	if(isOrtho){
		glOrtho(ort[0][0],ort[0][1],ort[1][0],ort[1][1],ort[2][0],ort[2][1]);
		//left, right, bottom, top, near, far
	}
	else{
		// Especifica a projeção perspectiva
		gluPerspective(angle,fAspect,0.5,500);
	}
	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador e do alvo
	gluLookAt(cam[0][0],cam[0][1],cam[0][2],cam[1][0],cam[1][1],cam[1][2],cam[2][0],cam[2][1],cam[2][2]);
	//gluLookAt([x,y,z]camera, [x,y,z]alvo, [x,y,z]vetor up)
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;

	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);
 
	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;

	EspecificaParametrosVisualizacao();
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-in
			if (angle >= 10) angle -= 5;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (angle <= 130) angle += 5;
		}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

// Função callback chamada para gerenciar eventos de teclas
void GerenciaTeclado(unsigned char key, int x, int y){
	//Esc
	if(key == 27){
		exit(0);
	}
	//o
	if(key == 111){
		isOrtho = !isOrtho;
	}


	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
	printf("near: %d\t far: %d\n", ort[2][0], ort[2][1]);
}

// left, right,   bottom, top,    near, far
// [x,y,z]camera, [x,y,z]alvo, [x,y,z]vetor up
void GerenciaEspeciais(int key, int x, int y){
	int alvoX, alvoZ,newX,newZ;
	switch(key){
		case GLUT_KEY_UP:
			
			cam[0][0] += STEP;
			cam[1][0] += STEP;
			
			break;
		case GLUT_KEY_DOWN:
			cam[0][0] -= STEP;
			cam[1][0] -= STEP;
			break;
		case GLUT_KEY_LEFT:
			cam[0][2] -= STEP;
			cam[1][2] -= STEP;
			break;
		case GLUT_KEY_RIGHT:
			// alvoX = cam[1][0] - cam[0][0];
			// alvoZ = cam[1][2] - cam[0][2];
			// newX = alvoX*cos(-alfa) + alvoZ*sin(-alfa);
			// newZ = -alvoX*sin(-alfa) + alvoZ*cos(-ALFA);
			// cam[1][0] = newX + cam[0][0];
			// cam[1][2] = newZ ;
			cam[0][2] += STEP;
			cam[1][2] += STEP;
			// printf("newX: %d\tnewZ: %d\n",cam[1][0],cam[1][2]);
			break;
	}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
	printf("bottom: %d\t top: %d\n", ort[1][0], ort[1][1]);
}

// Programa Principal 
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	// Define do modo de operação da GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000,800);
	glutCreateWindow("Visualizacao 3D");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutMouseFunc(GerenciaMouse);
	glutKeyboardFunc(GerenciaTeclado);
	glutSpecialFunc(GerenciaEspeciais);
	Inicializa();
	glutMainLoop();
}
