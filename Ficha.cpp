#include <stdio.h>
#include <stdlib.h>
#include<conio.h> // Usei para mudar a cor do texto
#include <locale.h> // Acentuação
#include <windows.h> // Biblioteca para Windows
#include <time.h> // Usado na função D20 por exemplo

int DX(int x){
    x = 1 + rand()%x;
    return x;
}

int D8(){
    int x;
    x = 1 + rand()%8;
    return x;
}

int D6(){
    int x;
    x = 1 + rand()%6;
    return x;
}

//struct criada para todas as fichas de inimigos
typedef struct fichaIni
{
	int Vida;
	int Def;
	int Atq;
	int Vel;
	int nivel;
	//comentado pois está sendo analisado se isso entrará no projeto final
	//char especie;
}inimigo;


// O ataque rápido usará o ataque do lutador X para aplicar dano ao adversário Y
// Retorna a nova vida do adversário Y
int AtaqueRapido(int XA, int Yvida, int YD){
    int x1=D6(); int x2=D6();
    int Dano = x1 + x2 + (XA)*2;
    int Negacao = (YD)*2;
    printf("\n\n\033[1;34m2 Dados d6 foram lançados...\n| 1º Dado: %d   +   2º Dado: %d |\033[1;0m\n\n", x1, x2);
    if(Dano<Negacao){return Yvida;}
    else{return Yvida-Dano+Negacao;}
}


// O ataque forte usará o ataque+2 do lutador X para aplicar dano ao adversário Y
// Retorna a nova vida do adversário Y
int AtaqueForte(int XA, int Yvida, int YD){
    int x1=D6(); int x2=D6();
    int Dano = x1 + x2 + (XA+2)*2;
    printf("\n\n\033[1;34m2 Dados d6 foram lançados...\n| 1º Dado: %d   +   2º Dado: %d |\033[1;0m\n\n", x1, x2);
    int Negacao = (YD)*2;
    if(Dano<Negacao){return Yvida;}
    else{return Yvida-Dano+Negacao;}
}

// Sua velocidade X versus a do inimigo Y com uma chance de negar todo o dano e dar um ataque rápido ou entÃ£o falhar e levar um ataque forte sem dar dano
int ContraAtaque(int XS, int YS){
    srand(time(NULL)); // Conserta o valor randomico para não repetir ao reexecutar o programa
    int x = 1 + rand()%(XS+YS);
    if(x<=XS){return 1;}else{return 0;}
}

// Cura 50 de vida
void Pocao(int *p, int *Vida, int VidaMax){
    if(*p>0){
        *p-=1;
        if(*Vida+50>VidaMax){*Vida=VidaMax;}
        else{*Vida+=50;}
        system("cls"); // Limpa a tela
        printf("\033[1;32mVocê usou uma poção para curar 50 de vida!\033[1;0m\n\n");
        system("pause");
    }
    else{
        system("cls"); // Limpa a tela
        printf("\033[1;35mSuas poções acabaram!\033[1;0m\n\n");
        system("pause");
    }
}

//Função para distribuir os pontos aleatoriamente do inimigo
void GeraIni(int *vida, int *def, int *atq, int *vel, int pontos)
{
	//scanf("%d", &*pontos);
	int atriSort;
	pontos  = (pontos)+1;
	for (int i = 0; i < pontos; i++)
  	{
    	/* gerando valores aleatórios entre zero e 3 */
    	atriSort = rand()%4;
    	switch(atriSort)
    	{
    		case 0:
    			(*vida)++;
    			break;
			case 1:
				(*def)++;
				break;
			case 2:
				(*vel)++;
				break;
			case 3:
				(*atq)++;
				break;	
		}		
  	}
  	
}

//função para a escolha da aplicação do ponto adiquirido na subida de nivel
void levelUp(int *PH, int *PD, int *PA, int *PS, int *vida)
{
	int esc;
	while(esc<1||esc>4){ // Valida a ação entre 1 e 4
	    system("cls"); // Limpa a tela
	    printf("Escolha o atributo que você deseja evoluir\n\n");
	    printf("1. \033[1;36mVida\033[1;0m    2. \033[1;36mAtaque\033[1;0m\n\n");
	    printf("3. \033[1;36mDefesa\033[1;0m    4. \033[1;36mVelocidade\033[1;0m\n\n");
	    printf("Escolha: ");
	    scanf("%d", &esc);
	}
	switch(esc)
	{
		case 1:
			(*PH)++;
			(*vida)+=20;
			break;
		case 2:
			(*PA)++;
			break;
		case 3:
			(*PD)++;
			break;
		case 4:
			(*PS)++;
			break;	
	}			
}


// PH,PD,PA,PS = Player... Health, Defense, Atack, Speed
void combate(int PH, int PD, int PA, int PS){ 
    int aux;
    int Vida = 80+(PH*20); // Vida será igual a 80 mais 20 para cada PH
    int V=0; // V = Vitórias
    int Exp=0; // exp a cada inimigo derrotado
    int N=1; // N = Nível
    int p=3; // P = Poções
    int act=0; // act = Variável para ação em jogo
    int drop; // variavel sobre a porcentagem de chance do inimigo dropar uma poção
    
    
    while(Vida>0){ // Enquanto o jogador estiver vivo
        int VidaMax=80+(PH*20); // Vida Máxima
        //variavel responsavel por criar um ponteiro do tipo inimigo
        inimigo* ini;
        //reservamos na memória um espaço para a ficha de um inimigo
		ini=(inimigo*)malloc(sizeof(inimigo));
		//atribuimos os pontos base do inimigo
		ini->Vida = 1;
		ini->Atq = 1;
		ini->Def = 1;
		ini->Vel = 1;
		//chamamos a função para distribuir os pontos restantes baseado no nivel do personagem
		GeraIni(&ini->Vida,&ini->Def,&ini->Atq,&ini->Vel, N);
        // atribuimos os pontos distribuidos a estas variaveis locais
        int EH=ini->Vida; int ED=ini->Def;
        int EA=ini->Atq; int ES=ini->Vel;
        // EH,ED,EA,Es = Enemy... Health, Defense, Atack, Speed
        int Evida=30+(EH*10);
        int EvidaMax=Evida;
        system("cls");
        printf("\033[1;33mUm inimigo apareceu!\033[1;0m\n\n");
        system("pause");
        
        while(Evida>0 && Vida>0){ // Enquanto o inimigo estiver vivo e você também
            act = 0; // Retorna a ação para 0 para poder reiniciar o looping da escolha de ação
            while(act<1||act>4){ // Valida a ação entre 1 e 4
                system("cls"); // Limpa a tela
                printf("|Sua Vida: \033[1;32m%d/%d\033[1;0m|      |Vida do Inimigo: \033[1;31m%d/%d\033[1;0m|\n\n", Vida, VidaMax, Evida, EvidaMax);
                printf("Escolha seu próximo movimento\n\n");
                printf("1. \033[1;36mAtaque Rápido\033[1;0m    2. \033[1;36mAtaque Forte\033[1;0m\n\n");
                printf("3. \033[1;36mContra-Ataque\033[1;0m    4. \033[1;36mPoção\033[1;0m (%d restantes)\n\n", p);
                printf("Ação: ");
                scanf("%d", &act);
            }

            if(act==4){Pocao(&p,&Vida,VidaMax);}
            
            if(PS<ES && act!=3){ // Se o inimigo for mais rápido e você não estiver usando um contra ataque, ele ataca primeiro
                system("cls");
                printf("\033[1;31mO inimigo foi mais veloz.\033[1;0m");
                aux=Vida;
                if(act==2){Vida = AtaqueRapido(EA, Vida, PD-2);}// Usar ataque forte diminui a defesa
                else{Vida = AtaqueRapido(EA, Vida, PD);}
                aux = aux - Vida;
                printf("Você recebe %d de dano.\n\n", aux);
                system("pause");
            }

            switch (act){
                case 1:
                system("cls");
                printf("\033[1;32mVocê usa um \033[1;33mataque rápido.\033[1;0m");
                aux=Evida;
                Evida = AtaqueRapido(PA, Evida, ED);
                aux = aux - Evida; // Usei apenas para contar o dano tomado pelo inimigo
                printf("\033[1;32mVocê causou %d de dano ao inimigo com um ataque rápido.\033[1;0m\n\n", aux);
                system("pause");
                break;
                
                case 2:
                
                system("cls");
                printf("\033[1;32mVocê usa um \033[1;33mataque forte.\033[1;0m");
                aux=Evida;
                Evida = AtaqueForte(PA, Evida, ED);
                aux = aux - Evida; // Usei apenas para contar o dano tomado pelo inimigo
                printf("\033[1;32mVocê usou um ataque forte e infligiu %d de dano ao inimigo.\033[1;0m\n\n", aux);
                system("pause");
                break;
                
                case 3:
                ContraAtaque(PS, ES);
                break;
            }

            if(PS>=ES && act!=3){ // Se o inimigo for mais devagar e vocÃª nÃ£o estiver usando um contra ataque, ele ataca primeiro
                system("cls");
                printf("\033[1;31mO inimigo te ataca.\033[1;0m");
                aux=Vida;
                if(act==2){Vida = AtaqueRapido(EA, Vida, PD-2);}// Usar ataque forte diminui a defesa
                else{Vida = AtaqueRapido(EA, Vida, PD);}
                aux = aux - Vida;
                printf("\033[1;31mVocê recebe %d de dano.\033[1;0m\n\n", aux);
                system("pause");
            }
        }
        //incrementa 1 vitória ao score
        V++;
        //incrementa um no nivel de experiencia
        Exp++;
        //verifica se a experiencia chegou no limite para ocorrer a evolução do nível
        if(Exp==2 && Vida>0)
        {
        	Exp = -2*(N);
        	N++;
        	system("cls");
        	printf("\033[1;37mVocê evoluiu de nível\033[1;0m\n\n");
        	system("pause");
        	levelUp(&PH, &PD, &PA, &PS, &Vida);
		}
        // comando para liberar o espaço da memória ocupado pelo inimigo morto
        free(ini);
        // verifica se inimigo esta morto, para prevenir que esta ação se o personagem morrer
        if(Vida>0)
        {
        	//sorteio para decidir se será dropado uma poção ou não (existe 33 por cento de chance)
        	drop = rand()%3;
        	switch(drop)
	        {
	        	case 0:
	        		system("cls");
	        		printf("\033[1;34mO inimigo deixa uma poção para trás\033[1;0m\n\n");
	        		p++;
	        		system("pause");
	        		break;
	        	case 1:
	        		break;
	        	case 2:
	        		break;
			}	
		}
		
    }
    system("cls");
    printf("Fim de Jogo...\nVocê Morreu\n\nInimigos Derrotados: %d", V);
}



int main()
{
    srand(time(NULL)); // Conserta o valor randomico para não repetir ao reexecutar o programa
    //comando para implementar musica e efeitos sonoros ao programa; por enquanto comentado pois está sob analise se será utilizado no projeto final.
    //PlaySound(TEXT("Zelda.wav"), NULL, SND_FILENAME | SND_ASYNC);
    setlocale(LC_ALL, "Portuguese");
    combate(2, 1, 2, 1);
     
    return 0;
}
