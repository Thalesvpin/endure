#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variáveis globais para armazenar os dados do objeto
GLfloat *vertices = NULL;
int numVertices = 0;
GLfloat angle=45;
GLfloat fAspect;

// [x,y,z]camera, [x,y,z]alvo, [x,y,z]vetor up
int cam[3][3] = { {0, 0, 0},
				  {0, 2, 0},
				  {0, 1, 0}
};

// Função para carregar o arquivo .obj
void loadObjFile(const char *filename) {
    printf("aqui\n");
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo .obj\n");
        exit(1);
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            numVertices++;
            vertices = (GLfloat *)realloc(vertices, numVertices * 3 * sizeof(GLfloat));
            sscanf(line, "v %f %f %f", &vertices[(numVertices - 1) * 3], &vertices[(numVertices - 1) * 3 + 1], &vertices[(numVertices - 1) * 3 + 2]);
        }
    }

    fclose(file);
}

// Função de renderização
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //glTranslatef(0.0f, 0.0f, -5.0f);

    // Renderize seu objeto aqui
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < numVertices; i++) {
        glVertex3f(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
    }
    glEnd();

    glutSwapBuffers();
}

void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

    // Especifica a projeção perspectiva
    gluPerspective(angle,fAspect,0.5,500);

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

// Função principal
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Uso: %s <arquivo.obj>\n", argv[0]);
        return 1;
    }

    loadObjFile(argv[1]);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1300, 800);
    glutCreateWindow("Objeto 3D");

    glutDisplayFunc(display);
    glutReshapeFunc(AlteraTamanhoJanela);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    free(vertices); // Libera a memória alocada para os vértices

    return 0;
}
