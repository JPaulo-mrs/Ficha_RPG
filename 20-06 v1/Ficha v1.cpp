#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // Usei para mudar a cor do texto
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
int Ataque(int XA, int Yvida, int YD){
    int x1=D6(); int x2=D6();
    int Dano = x1 + x2 + (XA)*2;
    int Negacao = (YD)*2;
    printf("\n\n\033[1;34m2 Dados d6 foram lançados...\n| 1º Dado:\033[1;33m %d   + \033[1;34m  2º Dado:\033[1;33m %d   + \033[1;34m  Bônus:\033[1;33m %d   - \033[1;34m  Defesa:\033[1;37m %d \033[1;34m|\033[1;0m\n\n", x1, x2, XA*2, YD*2);
    if(Dano<Negacao){return Yvida;}
    else{return Yvida-Dano+Negacao;}
}

// Sua velocidade X versus a do inimigo Y com uma chance de negar todo o dano e dar um ataque rápido ou entÃ£o falhar e levar um ataque forte sem dar dano
int ContraAtaque(int XS, int YS){
    srand(time(NULL)); // Conserta o valor randomico para não repetir ao reexecutar o programa
    int x = 1 + rand()%(XS*3+YS*3);
    if(x<=XS*3){return 1;}else{return 0;}
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
        printf("\033[1;95mSuas poções acabaram!\033[1;0m\n\n");
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
    int Luc=0;
    
    
    while(Vida>0){ // Enquanto o jogador estiver vivo
        int VidaMax=80+(PH*20); // Vida Máxima
        int vigor=PD;
        //variavel responsavel por criar um ponteiro do tipo inimigo
        inimigo* ini;
        //reservamos na memória um espaço para a ficha de um inimigo
		ini=(inimigo*)malloc(sizeof(inimigo));
		//atribuimos os pontos base do inimigo
		ini->Vida = 1;
		ini->Atq = 1;
		ini->Def = 1;
		ini->Vel = 1;
		
		if(V+1==16){
			Vida=VidaMax;
		}
		
		//chamamos a função para distribuir os pontos restantes baseado no nivel do personagem
		GeraIni(&ini->Vida,&ini->Def,&ini->Atq,&ini->Vel, N);
        // atribuimos os pontos distribuidos a estas variaveis locais
		int EH=ini->Vida; int ED=ini->Def;
    	int EA=ini->Atq; int ES=ini->Vel;
		if(V+1==5){ //4 pontos para lvl 2 boss = 5
			EH=3; ED=2;
        	EA=3; ES=2;
		}else if(V+1==10){ //7 pontos para lvl 5 boss = 8
			EH=7; ED=2;
        	EA=5; ES=3;
		}else if(V+1==15){ //10 pontos para lvl 8 cutulo = 12
			if(Luc==1){
				EH=12; ED=3;
        		EA=5; ES=2;
			}else{
				EH=60; ED=30;
        		EA=50; ES=20;
			}
		}else if(V+1==16){
			EH=60; ED=30;
        	EA=50; ES=20;
		}
        
        // EH,ED,EA,Es = Enemy... Health, Defense, Atack, Speed
        int Evida=30+(EH*10);
        int EvidaMax=Evida;
        system("cls");
        
        char Ename[60];
        
        if(V+1<=2){
        	char Ename[60]="Goblin";
		}else if(V+1<=4){
			char Ename[60]="Goblin Shaman";
		}else if(V+1==5){
			char Ename[60]="Rei Goblin";
		}else if(V+1<=7){
			char Ename[60]="Assassino";
		}else if(V+1<=9){
			char Ename[60]="Bárbaro";
		}else if(V+1==10){
			char Ename[60]="Wyvern";
		}else if(V+1<=12){
			char Ename[60]="Mago Ocultista";
		}else if(V+1<=14){
			char Ename[60]="Demônio Menor";
		}else if(V+1==15){
			if(Luc==1){
				char Ename[60]="\033[1;91mLúcifer";
			}else{
				char Ename[60]="\033[1;95mCthulhu";
			}
		}else if(V+1==16){
			char Ename[60]="\033[1;95mCthulhu";	
		}
		
		if(V+1!=5 && V+1!=10 && V+1!=15 && V+1!=16){ 
			printf("\033[1;33mUm \033[1;37m%s\033[1;33m será seu %dº inimigo!\033[1;0m\n\n", Ename, V+1);
		}
		else{
			printf("\033[1;33mBoss: \033[1;37m%s\033[1;33m surge e será seu %dº inimigo!\033[1;0m\n\n", Ename, V+1);
		}
        
        
        system("pause");
        
        while(Evida>0 && Vida>0){ // Enquanto o inimigo estiver vivo e você também
        	
            act = 0; // Retorna a ação para 0 para poder reiniciar o looping da escolha de ação
            while(act<1||act>4){ // Valida a ação entre 1 e 4
                system("cls"); // Limpa a tela
                printf("|Sua Vida: \033[1;32m%d/%d\033[1;0m|      |%s: \033[1;31m%d/%d\033[1;0m|\n\n", Vida, VidaMax, Ename, Evida, EvidaMax);
                printf("Escolha seu próximo movimento\n\n");
                printf("1. \033[1;36mAtaque Rápido\033[1;0m         2. \033[1;36mAtaque Forte\033[1;0m\n\n");
                printf("3. \033[1;36mContra-Ataque \033[1;0m(%d)\033[1;0m     4. \033[1;36mPoção\033[1;0m (%d)\n\n", vigor, p);
                printf("5. \033[1;36mVer Estatísticas\033[1;0m\n\n");
                printf("Ação: ");
                scanf("%d", &act);
                if(act==5){
                	system("cls");
					printf("| Seus Pontos: | Pontos do Inimigo: |\n");
					printf("|  Vida:\033[1;32m %d \033[1;0m    |  Vida:\033[1;31m %d \033[1;0m          |\n",PH,EH);
					printf("|  Ataque:\033[1;32m %d \033[1;0m  |  Ataque:\033[1;31m %d \033[1;0m        |\n",PA,EA);
					printf("|  Defesa:\033[1;32m %d \033[1;0m  |  Defesa:\033[1;31m %d \033[1;0m        |\n",PD,ED);
					printf("|  Veloc:\033[1;32m %d \033[1;0m   |  Veloc:\033[1;31m %d \033[1;0m         |\n\n",PS,ES);
					system("pause");
				}
            }

            if(act==4){Pocao(&p,&Vida,VidaMax);}
            
            if(PS<ES && act!=3){ // Se o inimigo for mais rápido e você não estiver usando um contra ataque, ele ataca primeiro
                system("cls");
                printf("\033[1;31mO inimigo foi mais veloz.\033[1;0m");
                aux=Vida;
                if(act==2){Vida = Ataque(EA+2, Vida, PD+1);}// Usar ataque forte diminui a defesa
                else{Vida = Ataque(EA, Vida, PD+1);}
                aux = aux - Vida;
                printf("\033[1;31mVocê recebe %d de dano.\033[1;0m\n\n", aux);
                system("pause");
            }
			
			if(Vida>0){
	            switch (act){
	                case 1:
	                system("cls");
	                printf("\033[1;32mVocê usa um \033[1;33mataque rápido.\033[1;0m");
	                aux=Evida;
	                Evida = Ataque(PA, Evida, ED);
	                aux = aux - Evida; // Usei apenas para contar o dano tomado pelo inimigo
	                printf("\033[1;32mVocê causou %d de dano ao inimigo com um ataque rápido.\033[1;0m\n\n", aux);
	                system("pause");
	                break;
	                
	                case 2:
	                
	                system("cls");
	                printf("\033[1;32mVocê usa um \033[1;33mataque forte.\033[1;0m");
	                aux=Evida;
	                Evida = Ataque(PA+2, Evida, ED);
	                aux = aux - Evida; // Usei apenas para contar o dano tomado pelo inimigo
	                printf("\033[1;32mVocê usou um ataque forte e infligiu %d de dano ao inimigo.\033[1;0m\n\n", aux);
	                system("pause");
	                break;
	                
	                case 3:
	                if(vigor>0){
		                int CA = ContraAtaque(PS, ES);
		                if(CA==1){
		                	system("cls");
		                	printf("\033[1;32mSua velocidade vence a do inimigo!\n\033[1;0m");
		                	printf("\033[1;32mVocê se defende do inimigo com sucesso e devolve um \033[1;33mataque forte.\033[1;0m");
		                	aux=Evida;
			                Evida = Ataque(PA+2, Evida, ED);
			                aux = aux - Evida; // Usei apenas para contar o dano tomado pelo inimigo
			                printf("\033[1;32mVocê causou %d de dano ao inimigo com um ataque forte.\033[1;0m\n\n", aux);
			                system("pause");
						}else{
							system("cls");
							printf("\033[1;31mNão foi veloz o suficiente!\n\033[1;0m");
			                printf("\033[1;31mSua defesa falha e o inimigo te acerta em cheio.\033[1;0m");
			                aux=Vida;
			                Vida = Ataque(EA+2, Vida, PD);
			                aux = aux - Vida;
			                printf("\033[1;31mVocê recebe %d de dano.\033[1;0m\n\n", aux);
			                system("pause");
						}
						vigor--;
					}else{
						system("cls");
	                	printf("\033[1;95mVocê está sem concentração para aplicar um contra-ataque.\033[1;0m\n\n");
		                system("pause");
					}
	                break;
	            }
        	}
			
			
            if(PS>=ES && act!=3 && Evida>0){ // Se o inimigo for mais devagar e vocÃª nÃ£o estiver usando um contra ataque, ele ataca primeiro
                system("cls");
                printf("\033[1;31mO inimigo te ataca.\033[1;0m");
                aux=Vida;
                if(act==2){Vida = Ataque(EA+2, Vida, PD+1);}// Usar ataque forte diminui a defesa
                else{Vida = Ataque(EA, Vida, PD+1);}
                aux = aux - Vida;
                printf("\033[1;31mVocê recebe %d de dano.\033[1;0m\n\n", aux);
                system("pause");
            }
        }
        
        
        if(Evida<=0){
        	V++;
	        if(V!=5 && V!=10 && V!=15){ 
				Exp++;
			}
			else{
				Exp+=2;
			}
	        system("cls");
	        printf("\033[1;32mVocê venceu o combate!\033[1;0m\n\n");
	        system("pause");
	        //verifica se a experiencia chegou no limite para ocorrer a evolução do nível
	        if(Exp==2)
	        {
	        	Exp=0;
	        	N++;
	        	system("cls");
	        	printf("\033[1;36mSubiu de Nível!\033[1;0m\n\n");
	        	system("pause");
	        	levelUp(&PH, &PD, &PA, &PS, &Vida);
			}
			
			if(Luc==1 && V==15){
				system("cls");
	        	printf("\033[1;36mVocê absorve a alma de Lúcifer \ne se torna um anjo de grau superior.\033[1;0m\n\n");
	        	printf("\033[1;36mTraga paz a este mundo!\033[1;0m\n\n");
	        	system("pause");
	        	
	        	PH=PH*5; PA=PA*10;
	        	PD=PD*10; PS=PS*10;
			}
		}
	        
        // comando para liberar o espaço da memória ocupado pelo inimigo morto
        free(ini);
        // verifica se inimigo esta morto, para prevenir que esta ação se o personagem morrer
        if(Vida>0)
        {
        	
        	if(V!=5 && V!=10 && V!=15){ 
				//sorteio para decidir se será dropado uma poção ou não (existe 33 por cento de chance)
	        	drop = rand()%3;
	        	switch(drop)
		        {
		        	case 0:
		        		system("cls");
		        		printf("\033[1;34mO inimigo deixa uma poção para trás.\033[1;0m\n\n");
		        		p++;
		        		system("pause");
		        		break;
		        	case 1:
		        		break;
		        	case 2:
		        		break;
				}
			}
			else{
				system("cls");
				printf("\033[1;34mO boss deixa 3 poções para trás.\033[1;0m\n\n");
				p+=3;
		        system("pause");
			}
        		
		}
		
		if(Vida<=0 && V==14 && Luc!=1){
			Vida=VidaMax;
			Luc=1;
			system("cls");
    		printf("\033[1;33mFim de Jogo...\nVocê Morreu\n\nInimigos Derrotados:\033[1;0m %d\n\n", V);
    		system("pause");
    		system("cls");
    		printf("\033[1;91mAs chamas do caos lhe ardem a pele.\nMas a esperança que em ti habita\n\033[1;0m");
    		printf("\033[1;91mbrilha mais que o fogo do inferno...\n\n\033[1;0m");
    		system("pause");
		}
		
		if(V==16){
	    	Vida=0;
		}
    }
    
    if(V==16){
    	system("cls");
    	printf("\033[1;96mFim de Jogo!\nO mundo que conhecemos chega a seu fim...\n\n\033[1;0m");
    	printf("\033[1;96mParabéns! Voce se torna o Deus de um novo looping existencial.\n\n\033[1;0m");
    	printf("\033[1;96mVocê é Khlûl'-hloo.\033[1;0m");
	}else{
		system("cls");
    	printf("\033[1;33mFim de Jogo...\nVocê Morreu\n\nInimigos Derrotados:\033[1;0m %d", V);
	}
    
    
}

int main()
{
    srand(time(NULL)); // Conserta o valor randomico para não repetir ao reexecutar o programa
    //comando para implementar musica e efeitos sonoros ao programa; por enquanto comentado pois está sob analise se será utilizado no projeto final.
    PlaySound(TEXT("Zelda.wav"), NULL, SND_FILENAME | SND_ASYNC);
    setlocale(LC_ALL, "Portuguese");
    combate(2, 2, 2, 2);
     
    return 0;
}
