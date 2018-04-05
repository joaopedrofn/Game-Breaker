#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>

// ----- Resolucao da Tela
#define ScreenW 800.0
#define ScreenH 600.0

struct atributosBloco{

	float posicaoX;
	float posicaoY;
	int status;
	
};
struct atributosBola{
	
	float posicaoX;
	float posicaoY;
	float velocX;
	float velocY;
	
};

struct atributosBarra{
	
	float posicaoX;
	float posicaoY;
	float velocidade;
};

// ----- Variaveis Globais

int xAntigo;
int yAntigo;

int temporizador = 0;					// Conta quanto tempo leva da primeira colisão com um bloco para a segunda
int vidas = 3;							// Quantidade de vidas
unsigned int spacePressed = 0; 			// So comeca o jogo quando o jogador aperta Barra de ESpaco
unsigned int fase = 1;					// Numero da fase
unsigned int pontosBloco = 0;			// adiciona ponto(s) para cada bloco quebrado
unsigned int pontoFinal = 0;


float playing;

struct atributosBloco bloco[8][13]; 	// instancia os blocos na tela
struct atributosBola bola;				// Variavel bola 
struct atributosBarra barra;			// Variavel barra
 
int opcao = 1;							// Opcao do Menu 
int comecarJogar = 0;					// Só comeca o jogo quando o jogar escolhe a opcao JOGAR no menu
int nivelMenu = 1;						// Define o menu queo jogador se encontra atualmente

volatile int closewindow = 0;

// ----- Funcoes   Utilizadas

void moverDireita();  				// Mover Barra DIREITA
void moverEsquerda();				// Mover Barra ESQUERDA
void gerarBloco();					// Gera os blocos, e tipos de blocos
void CloseButtonCallback();    		// FUNCAO ESPECIFICA DO ALLEGRO
void inicializar();			    	// Inicia todas as funcoes da biblioteca Allegro
void detectarColisoes();			// Detecta Colisões Bola -> Bloco
void iniciarVariaveis();			// Define valores padrão das variáveis
void pontuacao(int tipoBloco);		// Calcula a pontuacao do jogador
int menu(BITMAP * Buffer);			// Funcao que mostra o menu
void salvar();						// Funcao qeu salva a pontuacao
void lerScore(BITMAP *Buffer);		// Funcao que escreve os maiores scores, ja feitos no jogo,na tela
int keyrel(int k);					


//==================================================================
//==================================================================
//==================================================================
//=======================   FUNCAO MAIN   ==========================
//==================================================================
//==================================================================
//==================================================================
int main()
{
	

	inicializar();
	iniciarVariaveis();

	// ----- Variaveis Locais

		int i, j, k;
		int novoJogo;		// Retorno do MENU, que diz se o jogador esta comecando um novo jogo ou NAO, se nao , ele nao tera direito as TECLAS ESPECIAIS
     			  
	// ---- Criando e Carregando BITMAPS
		 BITMAP *Buffer = create_bitmap(ScreenW, ScreenH);
		 BITMAP *barraH[2];
			barraH[0] = load_bitmap("imagens/barra_1.bmp", NULL);
			barraH[1] = load_bitmap("imagens/barra_2.bmp", NULL);
		 BITMAP *vida = load_bitmap("imagens/virus_bola.bmp", NULL);

		  BITMAP *B_bola[4]; 
			B_bola[0] =load_bitmap("imagens/bola_mario.bmp", NULL);
			B_bola[1] =load_bitmap("imagens/bola_sonic.bmp", NULL);
			B_bola[2] =load_bitmap("imagens/bola_zelda.bmp", NULL);
			B_bola[3] =load_bitmap("imagens/bola_pacman.bmp", NULL);
				  
		BITMAP *bloco_bmp[5] ;
			bloco_bmp[0] = load_bitmap("imagens/bloco1.bmp", NULL);
			bloco_bmp[1] = load_bitmap("imagens/bloco2.bmp", NULL);
			bloco_bmp[2] = load_bitmap("imagens/bloco3.bmp", NULL);
			bloco_bmp[3] = load_bitmap("imagens/bloco4.bmp", NULL);
			bloco_bmp[4] = load_bitmap("imagens/bloco5.bmp", NULL);

		BITMAP *cabecalho[5];
		    cabecalho[0] = load_bitmap("imagens/cabecalho_mario.bmp",NULL);
			cabecalho[1] = load_bitmap("imagens/cabecalho_sonic.bmp",NULL);
			cabecalho[2] = load_bitmap("imagens/cabecalho_zelda.bmp",NULL);
			cabecalho[3] = load_bitmap("imagens/cabecalho_pacman.bmp",NULL);
			cabecalho[4] = load_bitmap("imagens/cabecalho_gandalf.bmp",NULL);
	     BITMAP *bk[5];
			bk[0] = load_bitmap("imagens/bk_mario.bmp",NULL);	
			bk[1] = load_bitmap("imagens/bk_sonic.bmp",NULL);
			bk[2] = load_bitmap("imagens/bk_zelda.bmp",NULL);
			bk[3] = load_bitmap("imagens/bk_pacman.bmp",NULL);
			bk[4] = load_bitmap("imagens/bk_gandalf.bmp",NULL);
				
	  	  BITMAP *game_over = load_bitmap("imagens/gameover.bmp", NULL);
		  BITMAP *pause = load_bitmap("imagens/bk_pause.bmp",NULL);

		  FONT *font_pontos = load_font("Fonts/font_score.pcx",NULL,NULL);
		  FONT *font_pontos2 = load_font("Fonts/font_score2.pcx",NULL,NULL);

		  BITMAP *win = load_bitmap("imagens/win.bmp", NULL);
		
	novoJogo = menu(Buffer);	//	Carregando Menu
	gerarBloco();	// ----- Chamando a funcao que gera os blocos

	//######################################################
	//###################    Game Loop     #################
	//######################################################

	int counter = 1;	// Contador da barra de tesla para fazer a ANIMACAO
	int contadorX= 0;		//Contadore da TECLA ESPECIAL X pressioanda;
	int contadorC= 0;		//Contadore da TECLA ESPECIAL C pressioanda;
	float tmpBolaVX;	// Armazena temporariamente a velocidade da bola (USADO PELA AS TECLA ESPECIAL  X)
	float tmpBolaVY;	// Armazena temporariamente a velocidade da bola (USADO PELA AS TECLA ESPECIAL  C)
	

	while(!closewindow && comecarJogar)
	{
		clear_bitmap(Buffer);
		xAntigo = bola.posicaoX;
		yAntigo = bola.posicaoY;
		temporizador ++;
		

		// TECLAS ESPECIAIS  X e C
			int especialX, especialC;
		
		// GAME OVER
		if (vidas < 0){

			pontoFinal += pontosBloco;
			salvar();

			while(!key[KEY_ESC]){
				clear_bitmap(Buffer);
				draw_sprite(Buffer, game_over,0,0);
				
				textprintf_ex(Buffer,font_pontos2,500,10,makecol(255,255,255),-1,"PONTUACAO:%d",pontoFinal);
				draw_sprite(screen, Buffer,0,0);

			}

			iniciarVariaveis();
			vidas = 3;
			gerarBloco();
			pontosBloco = 0;
			comecarJogar = 0;
			nivelMenu = 1;
			menu(Buffer);
			break;
		}
		// SE A FASE MAIOR QUE 5 JOGADOR GANAH O JOGO
		if(fase > 5)
		{
		pontoFinal += pontosBloco;
		salvar();

			while(!key[KEY_ESC])
			{
				clear_bitmap(Buffer);
				
				draw_sprite(Buffer, win,0,0);
				textprintf_ex(Buffer,font_pontos2,500,10,makecol(255,255,255),-1,"PONTUACAO:%d",pontoFinal);
				
				draw_sprite(screen, Buffer,0,0);
			}
			iniciarVariaveis();
			vidas = 3;
			gerarBloco();
			pontosBloco = 0;
			comecarJogar = 0;
			nivelMenu = 1;
			menu(Buffer);
			break;
				
		}
		
		//===============================
		// TECLAS DE CONTROLE
		if(key[KEY_RIGHT])
			moverDireita();
		if(key[KEY_LEFT])
			moverEsquerda();
		if(key[KEY_SPACE])
			 spacePressed = 1;
	
		// Opcoa de PAUSAR O JOGO
				if(keyrel(KEY_P) || keyrel(KEY_ESC))
				{
					while(!keyrel(KEY_P))
					{
						if(keyrel(KEY_ESC))
						{
							closewindow = 1;
							break;
						}

						draw_sprite(screen,pause,0,0);
				
						//clear_bitmap(screen);
					}
				}

		// CONDICAO DAS TECLAS ESPECIAIS
				
				if(fase == 2 && vidas == 3 && novoJogo)
					especialX = 1;
				if(fase == 4 && vidas == 3 && novoJogo)
					especialC = 1;

				//AUMNETA A GRAVIDADE EM RELACAO AO EIXO Y (TECLA ESPECIAL C)
				if(key[KEY_C] && bola.velocY <= 10.0 && especialC)
				{
					if(!contadorC)
						{
							tmpBolaVY = bola.velocY;
							contadorC = 1;
						}
					if(bola.velocY > 0)
						bola.velocY+= 0.5;
					else
						bola.velocY-= 0.5;
				}
					
					//RESTAURA O VALOR ANTIGO DE bola.velocY
						if(keyrel(KEY_C) && especialC)
						{
							bola.velocY = tmpBolaVY;
							contadorC = 0;
						}

				//AUMENTA VELOCIDADE EM RELACAO AO EIXO X (TECLA ESPECIAL X)
					if(key[KEY_X] && bola.velocX <= 8.0 && especialX)
					{
						if(!contadorX)
						{
							tmpBolaVX = bola.velocX;
							contadorX = 1;
						}

						if(bola.velocX > 0)
							bola.velocX+= 0.5;
						else
							bola.velocX-= 0.5;

					
					}
					//RESTAURA O VALOR ANTIGO DE bola.velocX
					if(keyrel(KEY_X) && especialX)
					{
							bola.velocX = tmpBolaVX;
							contadorX = 0;
					}
		//===========================================================================
		//===========================================================================
		//=====================   DESENHANDO NO BUFFER E NA SCREEN  =================
		//===========================================================================
		//===========================================================================
		//===========================================================================
					
		
		// Desenha BACKGROUND
		switch(fase)
		{
			case 1:
				draw_sprite(Buffer, bk[0], 0, 0);
				draw_sprite(Buffer, cabecalho[0], 0, 0);
				break;
			case 2:
				draw_sprite(Buffer, bk[1], 0, 0);
				draw_sprite(Buffer, cabecalho[1], 0, 0);
				break;
			case 3:
				draw_sprite(Buffer, bk[2], 0, 0);
				draw_sprite(Buffer, cabecalho[2], 0, 0);
				break;
			case 4:
				draw_sprite(Buffer, bk[3], 0, 0);
				draw_sprite(Buffer, cabecalho[3], 0, 0);
				break;
			case 5:
				draw_sprite(Buffer, bk[4], 0, 0);
				draw_sprite(Buffer, cabecalho[4], 0, 0);
				break;
		}
		

		// ---- Desenha a barra
		if(counter == 1)
		{
			draw_sprite(Buffer, barraH[0], barra.posicaoX, barra.posicaoY);
			counter = 2;
			rest(30);
		}	
		else
		{
			draw_sprite(Buffer, barraH[1], barra.posicaoX, barra.posicaoY);
			counter = 1;
			rest(30);
		}
			
				
		//=====================================================================================================
		//
		// Desenha a bola
		switch(fase)
		{
			case 1:
				draw_sprite(Buffer, B_bola[0], bola.posicaoX, bola.posicaoY);
				break;
			case 2:
				draw_sprite(Buffer, B_bola[1], bola.posicaoX, bola.posicaoY);
				break;
			case 3:
				draw_sprite(Buffer, B_bola[2], bola.posicaoX, bola.posicaoY);
				break;	
			case 4:
				draw_sprite(Buffer, B_bola[3], bola.posicaoX, bola.posicaoY);
				break;	
			case 5:
				draw_sprite(Buffer, vida, bola.posicaoX, bola.posicaoY);
				break;	
		}
		
		  
		// Desenha os blocos
				
		for (j=0; j<8; j++){
			for (k=0; k<13; k++){
				if(bloco[j][k].status>0)
					draw_sprite(Buffer, bloco_bmp[bloco[j][k].status-1],bloco[j][k].posicaoX, bloco[j][k].posicaoY); 
			}
		}
		// Desenha a(s) vida(s)
			// ----- Desenha Cenário, Vida, Pontos
			
		
				// ----- Desenha as vidas
				int x =30;
				for(i = 0; i < vidas; i++)
				{
					draw_sprite(Buffer, vida,x,15);
					x+= 30;
				}


		// Desenha o SCORE na tela
				textprintf_ex(Buffer, font_pontos, 690, 10, makecol(0,0,255),-1,"%d", pontosBloco);
		
		
		

			draw_sprite(screen,Buffer,0,0);
		


		//===========================================================================
		//===========================================================================
		//================  TERMINA DE DESENHAR NO BUFFER E NA SCREEN  ==============
		//===========================================================================
		//===========================================================================

		//===========================================================================
		//====================== COMPORTAMENTO DOS BLOCOS ===========================
		//===========================================================================
		//===========================================================================
			for (j=0; j<8; j++){
			for (k=0; k<13; k++){
				
				

				//   PARTE INFERIOR DO BLOCO
				if(   (bola.posicaoX+12.5 >= bloco[j][k].posicaoX  && bola.posicaoX+12.5 <=  bloco[j][k].posicaoX + 60.0) && (bola.posicaoY <= bloco[j][k].posicaoY+25.0 && bola.posicaoY >= bloco[j][k].posicaoY+12.5 )&& bloco[j][k].status)
				{
					pontuacao(bloco[j][k].status);
					bloco[j][k].status--;
					
					barra.velocidade += 0.06;
					if(bola.velocX < 0)
						bola.velocX -= 0.1;
					else
						bola.velocX += 0.1;
					bola.velocY -= 0.05;
					bola.velocY = -bola.velocY;
					break;
				}
				//PARTE SUPERIOR DO BLOCO
				else if( (bola.posicaoX+12.5 >= bloco[j][k].posicaoX  && bola.posicaoX+12.5 <=  bloco[j][k].posicaoX + 60.0) && (bola.posicaoY+25.0 <= bloco[j][k].posicaoY+12.5 && bola.posicaoY >= bloco[j][k].posicaoY-25.0 )&& bloco[j][k].status)
				{
					pontuacao(bloco[j][k].status);
					bloco[j][k].status--;
					
					barra.velocidade += 0.06;
					
					if(bola.velocX < 0)
						bola.velocX -= 0.1;
					else
						bola.velocX += 0.1;
					bola.velocY -= 0.05;
					
					bola.velocY = -bola.velocY;
					break;
				}
				//LATERAL DIREITA
				else if( (bola.posicaoX+25 >= bloco[j][k].posicaoX-12.5  && bola.posicaoX+25 <=  bloco[j][k].posicaoX) && (bola.posicaoY+12.5 <= bloco[j][k].posicaoY+25 && bola.posicaoY+12.5 >= bloco[j][k].posicaoY )&& bloco[j][k].status)
				{
					pontuacao(bloco[j][k].status);
					bloco[j][k].status--;
					
					barra.velocidade += 0.06;
					bola.velocX += 0.1;
					if(bola.velocY < 0)
						bola.velocY -= 0.05;
					else
						bola.velocY += 0.05;
					bola.velocX = -bola.velocX;
					break;
				}
				//LATERAL ESQUERDA
				else	if( (bola.posicaoX >= bloco[j][k].posicaoX+60  && bola.posicaoX <=  bloco[j][k].posicaoX+72.5) && (bola.posicaoY+12.5 <= bloco[j][k].posicaoY+25 && bola.posicaoY+12.5 >= bloco[j][k].posicaoY )&& bloco[j][k].status)
				{
					pontuacao(bloco[j][k].status);
					bloco[j][k].status--;
					
					barra.velocidade += 0.06;
					bola.velocX += 0.1;
					if(bola.velocY < 0)
						bola.velocY -= 0.05;
					else
						bola.velocY += 0.05;
					bola.velocX = -bola.velocX;
					break;
				}
				// **CASO ESPECIAIS**
				// QUINA ESQUERDA SUPERIOR
				else if(   (bola.posicaoX >= bloco[j][k].posicaoX+50.0 && bola.posicaoX <=  bloco[j][k].posicaoX+70.0) && (bola.posicaoY <= bloco[j][k].posicaoY+25.0 && bola.posicaoY >= bloco[j][k].posicaoY+13.0 )&& bloco[j][k].status)
				{
					pontuacao(bloco[j][k].status);
					bloco[j][k].status--;
					
					barra.velocidade += 0.06;
					if(bola.velocX < 0)
						bola.velocX -= 0.1;
					else
						bola.velocX += 0.1;
					bola.velocY -= 0.05;
					bola.velocY = -bola.velocY;
					break;
				}
				// QUINA DIREITA SUPERIOR
				else if(   (bola.posicaoX+25 >= bloco[j][k].posicaoX-10.0 && bola.posicaoX+25.0 <=  bloco[j][k].posicaoX+10.0) && (bola.posicaoY <= bloco[j][k].posicaoY+25.0 && bola.posicaoY >= bloco[j][k].posicaoY+13.0 )&& bloco[j][k].status)
				{
					pontuacao(bloco[j][k].status);
					bloco[j][k].status--;
					
					bola.posicaoY = bloco[j][k].posicaoY+26.0;
					barra.velocidade += 0.06;
					if(bola.velocX < 0)
						bola.velocX -= 0.1;
					else
						bola.velocX += 0.1;
					bola.velocY -= 0.05;
					bola.velocY = -bola.velocY;
					break;
				}
				// QUINA ESQUERDA INFERIOR
				else if(   (bola.posicaoX >= bloco[j][k].posicaoX+50.0 && bola.posicaoX <=  bloco[j][k].posicaoX+70.0) && (bola.posicaoY+25.0 <= bloco[j][k].posicaoY+13.0 && bola.posicaoY+25.0 >= bloco[j][k].posicaoY )&& bloco[j][k].status)
				{
					pontuacao(bloco[j][k].status);
					bloco[j][k].status--;
					
					bola.posicaoY = bloco[j][k].posicaoY+26.0;
					barra.velocidade += 0.7;
					if(bola.velocX < 0)
						bola.velocX -= 0.1;
					else
						bola.velocX += 0.1;
					bola.velocY -= 0.05;
					bola.velocY = -bola.velocY;
					break;
				}
				// QUINA DIREITA INFERIOR
				else if(   (bola.posicaoX+25.0 >= bloco[j][k].posicaoX-10.0 && bola.posicaoX+25.0 <=  bloco[j][k].posicaoX+10.0) && (bola.posicaoY+25.0 <= bloco[j][k].posicaoY+13 && bola.posicaoY >= bloco[j][k].posicaoY )&& bloco[j][k].status)
				{
					pontuacao(bloco[j][k].status);
					bloco[j][k].status--;
					bola.posicaoX = bloco[j][k].posicaoX -26.0;
					barra.velocidade += 0.7;
					if(bola.velocX < 0)
						bola.velocX -= 0.1;
					else
						bola.velocX += 0.1;
					bola.velocY -= 0.05;
					bola.velocY = -bola.velocY;
					break;
				}

				
			}
		}
		
		//===========================================================================
		//====================== MOVIMENTACAO DA BOLA ===============================
		//===========================================================================
		// ----- Gravidade da bola
		//===========================================================================
	if(spacePressed)
	{
				
			bola.posicaoX += bola.velocX ;
			bola.posicaoY += bola.velocY ;
			detectarColisoes();
				 
	}
	
		//===========================================================================
		//================== TERMINA A MOVIMENTACAO DA BOLA =========================
		//===========================================================================
		 
		//===========================================================================
		//===========================================================================
		//=========================   MUDANÇA DE FASE  ==============================
		//===========================================================================
		//===========================================================================
		//===========================================================================
	int checar = 0;
	for (j=0; j<8; j++){
		for (k=0; k<13; k++){
			
			checar += bloco[j][k].status;
		
		}
	}
	
	if(!checar){

		fase ++;
		pontoFinal += pontosBloco;
		iniciarVariaveis();
		gerarBloco();
	}
				






    //===========================================================================
	//================  LIMPA TELA E RENICIA CICLO  =============================
	//===========================================================================
		
		
	clear_bitmap(Buffer);

//FIM DO WHILE	
}

		
	//===========================================================================	
	//================   FINALIZANDO PROGRAMA   =================================
	//===========================================================================
		
			
	destroy_bitmap(Buffer);
	//-----
	for(i =0; i < 2; i++)
	{
		destroy_bitmap(barraH[i]);
	}	
	
	//--
	for(i =0; i < 5; i++)
	{
		destroy_bitmap(bk[i]);
	}
	//--
	for(i =0; i < 4; i++)
	{
		destroy_bitmap(B_bola[i]);
	}
	
	destroy_bitmap(vida);
	destroy_bitmap(win);

	//--
	for(i =0; i < 5; i++)
	{
		destroy_bitmap(cabecalho[i]);
	}
	//--
	destroy_bitmap(game_over);
	//=========================
	// Destroi fontes
	destroy_font(font_pontos);
	destroy_font(font_pontos2);

	

	allegro_exit();

	//=================================
	return 0;

}
END_OF_MAIN()
//========================================================
//===================== FUNCOES ==========================
//========================================================

void inicializar()
{
	allegro_init();
	install_keyboard();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,ScreenW,ScreenH,0,0);
	set_window_title("Game Breaker");
	
	
	LOCK_FUNCTION(CloseButtonCallback);
	LOCK_VARIABLE(closewindow);
		
	
	set_close_button_callback(CloseButtonCallback);

}
END_OF_FUNCTION(inicializar);
int menu(BITMAP * Buffer)
{
	int i;
	// Carrega as imagens do Menu
	BITMAP *menuPrincipal[4];
			menuPrincipal[0] = load_bitmap("imagens/menu01.bmp", NULL);
			menuPrincipal[1] = load_bitmap("imagens/menu02.bmp", NULL);
			menuPrincipal[2] = load_bitmap("imagens/menu03.bmp", NULL);
			menuPrincipal[3] = load_bitmap("imagens/menu04.bmp", NULL);
		BITMAP *menuSecundario[2];
			menuSecundario[0] = load_bitmap("imagens/menujogar01.bmp", NULL);
			menuSecundario[1] = load_bitmap("imagens/menujogar02.bmp", NULL);
		BITMAP *menuNivel[5];
			menuNivel[0] = load_bitmap("imagens/menunivel01.bmp", NULL);
			menuNivel[1] = load_bitmap("imagens/menunivel02.bmp", NULL);
			menuNivel[2] = load_bitmap("imagens/menunivel03.bmp", NULL);
			menuNivel[3] = load_bitmap("imagens/menunivel04.bmp", NULL);
			menuNivel[4] = load_bitmap("imagens/menunivel05.bmp", NULL);
		BITMAP *tutorial = load_bitmap("imagens/tutorial.bmp", NULL);
		BITMAP *scores = load_bitmap("imagens/menuScore.bmp", NULL);

	// Interações com o Menu 
	while(!closewindow && !comecarJogar)
	{
		

		// --- TELA INICIAL
	if(keyrel(KEY_DOWN) && opcao < 5 )
		opcao ++;
				
	if(keyrel(KEY_UP) && opcao > 1)
		opcao --;

	// CADA CASE DESSE SWITCH EH UM NIVEL DO MENU, CONTANDO COM 4 NIVEIS NO TOTAL
	switch(nivelMenu)
	{
		case 1:
						//====================================	
						//====================================
						//===========MENU PRINCIPAL===========
						//====================================
						//====================================
						
						switch(opcao)
						{
							case  1:
								draw_sprite(Buffer, menuPrincipal[0],0,0);
								break;
							case  2:
								draw_sprite(Buffer, menuPrincipal[1],0,0);
								break;
							case  3:
								draw_sprite(Buffer, menuPrincipal[2],0,0);
								break;
							case  4:
							default: 
								draw_sprite(Buffer, menuPrincipal[3],0,0);
								opcao = 4;
								break;
			
						}
						if(keyrel(KEY_ENTER))
						{
							switch(opcao)
							{
								case 1:
									nivelMenu =2;
									opcao = 1;
									break;
								case 2:
									nivelMenu = 5;
									break;
								case 3:
									nivelMenu = 3;
									break;
								case 4:
									exit(0);
							}
	
	
						}
	
		break;

	case 2:
	// MENU SECUNDARIO, JOGAR E SELECIONAR NIVEL
				if(keyrel(KEY_ESC))
				{
					nivelMenu --;
					opcao = 1;
				}
				switch(opcao)
				{
									case  1:
										draw_sprite(Buffer,menuSecundario[0], 0, 0);
										break;
									case  2:
										draw_sprite(Buffer, menuSecundario[1], 0, 0);
										break;
									default: 
										draw_sprite(Buffer, menuSecundario[1], 0, 0);
										opcao = 2;
										break;
							
			
				}
				if(keyrel(KEY_ENTER))
				{
					switch(opcao)
					{
						case 1:
							comecarJogar = 1;
							return 1;
							break;
					
						case 2:
							nivelMenu = 4;
							opcao =1;
							break;
				
						}
	
	
				}
		
	break;

	case 3:
	//MENU TUTORIAL
		
		if(keyrel(KEY_ESC))
		{
				nivelMenu = 1;
				opcao = 1;
		}

		draw_sprite(Buffer, tutorial, 0, 0);
		break;
	
		
	case 4:
	//MENU SELECIONAR NIVEL
		if(keyrel(KEY_ESC))
		{
				nivelMenu = 2;
				opcao = 1;
		}
		switch(opcao)
				{
						case  1:
							draw_sprite(Buffer, menuNivel[0], 0, 0);
							break;
						case  2:
							draw_sprite(Buffer, menuNivel[1], 0, 0);
							break;
						case  3:
							draw_sprite(Buffer, menuNivel[2], 0, 0);
							break;
						case  4:
							draw_sprite(Buffer, menuNivel[3], 0, 0);
							break;
						case  5:
							draw_sprite(Buffer, menuNivel[4], 0, 0);
							break;
						default:
							draw_sprite(Buffer, menuNivel[4], 0, 0);
							opcao = 5;
							break;

							
			
				}
				if(keyrel(KEY_ENTER))
				{
					switch(opcao)
					{
						case 1:
							comecarJogar = 1;
							draw_sprite(Buffer, menuNivel[0], 0, 0);
							fase = 1;
							return 1;
							break;
					
						case 2:
							comecarJogar = 1;
							draw_sprite(Buffer, menuNivel[1], 0, 0);
							fase = 2;
							return 0;
							break;

						case 3:
							comecarJogar = 1;
							draw_sprite(Buffer, menuNivel[2], 0, 0);
							fase = 3;
							return 0;
							break;

						case 4:
							comecarJogar = 1;
							draw_sprite(Buffer, menuNivel[3], 0, 0);
							fase = 4;
							return 0;
							break;

						case 5:
							comecarJogar = 1;
							draw_sprite(Buffer, menuNivel[4], 0, 0);
							fase = 5;
							return 0;
							break;
				
						}
	
	
				}

		break;

	case 5:
	// MENU SCORES

		if(keyrel(KEY_ESC))
		{
				nivelMenu = 1;
				opcao = 1;
		}
				draw_sprite(Buffer, scores, 0, 0);
				lerScore(Buffer);
				
		
		break;
	}
	
	
	draw_sprite(screen,Buffer,0,0);
	clear_bitmap(Buffer);
}
	if(comecarJogar)
	{
			//-----
			for(i =0; i < 4; i++)
			{
				destroy_bitmap(menuPrincipal[i]);
			}
			for(i =0; i < 2; i++)
			{
				destroy_bitmap(menuSecundario[i]);
			}
			for(i =0; i < 5; i++)
			{
				destroy_bitmap(menuNivel[i]);
			}
		destroy_bitmap(tutorial);
	}

}
END_OF_FUNCTION(menu);
void moverDireita()
{
	if(barra.posicaoX >= ScreenW -90 && !spacePressed){
		barra.posicaoX = ScreenW - 90;
		bola.posicaoX = barra.posicaoX + 25;
	}else if(barra.posicaoX >= ScreenW -90)
		barra.posicaoX = ScreenW - 90;

	barra.posicaoX += barra.velocidade;
	// Se o jogo ainda nao comecou, a bola anda junto (em cima) da barra
	if(!spacePressed)
		bola.posicaoX += barra.velocidade;
	
}

END_OF_FUNCTION(moverDireita);
void moverEsquerda()
{
	
	if(barra.posicaoX <= 10 && !spacePressed){
		barra.posicaoX = 10;
		bola.posicaoX = barra.posicaoX + 25;
	}else if(barra.posicaoX <= 10)
		barra.posicaoX = 10;
	barra.posicaoX-= barra.velocidade;
	// Se o jogo ainda nao comecou, a bola anda junto (em cima) da barra
	if(!spacePressed)
		bola.posicaoX -= barra.velocidade;
	
}
END_OF_FUNCTION(moverEsquerda);
        // ----- Fim da Movimentacao da Barra Horizontal

// ------ Funcao que gera os blocos na tela

void gerarBloco (){
	int j, k;
	for (j = 0; j<8; j++){
		for (k = 0; k<13; k++){
			bloco[j][k].posicaoX = (k*60)+10;
			bloco[j][k].posicaoY = (j*25)+80;
			bloco[j][k].status = (rand()%fase)+1;
			
		}
	}
}
END_OF_FUNCTION(gerarBloco);
void iniciarVariaveis(){

						bola.posicaoX = ScreenW/2 - 25.0;
						bola.posicaoY = 538.0;
						bola.velocY = 6.0;
						bola.velocX = 6.0;
						barra.posicaoX = ScreenW/2 - 50.0;
						barra.posicaoY = 560;
						barra.velocidade = 10.0;
						spacePressed = 0;
						playing = 0.0;
						
}
END_OF_FUNCTION(iniciarVariaveis);


// ---- Funcao pontuacao
void pontuacao(int tipoBloco){
	
	pontosBloco += 2*tipoBloco;
	

	//FILE *arqScore;
	//if((arqScore = fopen("","")))

}
END_OF_FUNCTION(pontuacao);
// ------- Funcao que detecta colisoes
void detectarColisoes(){

	
	// --- Se a bola passar da barra voce perde uma vida
					if(bola.posicaoY > ScreenH)
					{
						vidas --;
						iniciarVariaveis();
						
					}
			
		
			// ---- Checa de a bola REALMENTE esta colidindo com a barra horizontal, muda a direcao da bola e aumenta a velocidade de movimento
				
				// PARTE CENRAL DA BARRA
				if( (bola.posicaoX+12.5 >= barra.posicaoX && bola.posicaoX <= barra.posicaoX+80) && (bola.posicaoY+25 >= barra.posicaoY-2.0 && bola.posicaoY+25 <= barra.posicaoY+25.0 ) && (playing >= 1.0))
				{

					//A BARRA FOI DIVIDIDA EM 5 PARTES
					//PARTE 1 ESQUERDA
					if( bola.posicaoX+12.5 >= barra.posicaoX && bola.posicaoX+12.5 < barra.posicaoX+12.5 ) 
					{
						
						if(bola.velocX < 0)
							bola.velocX -=1.2;
						else
							bola.velocX +=1.0;
					}
					//PARTE 2 E 4 INTERMEDIARIAS ENTRE LATERAIS E CENTRAL
					else if( (bola.posicaoX+12.5 >= barra.posicaoX+12.5 && bola.posicaoX+12.5 < barra.posicaoX +22.5 ) || (bola.posicaoX+12.5 >= barra.posicaoX+32.5 && barra.posicaoX+12.5 < barra.posicaoX+47.5))
					{
						if(bola.velocX < 0)
							bola.velocX +=0.7 ;
						if(bola.velocX > 0)
								bola.velocX -=0.7 ;
					}
					// PARTE 3 CENTRAL
					else if( bola.posicaoX+12.5 >=  barra.posicaoX+22.5 && bola.posicaoX+12.5 < barra.posicaoX +32.5)
					{
						if(bola.velocX < 0)
							bola.velocX +=2.0 ;
						if(bola.velocX > 0)
								bola.velocX -=2.0 ;
					}
					// PARTE 5 DIRETIA
					else if( bola.posicaoX+12.5 >= barra.posicaoX+47.5 && bola.posicaoX+12.5 < barra.posicaoX+60.0) 
					{
						if(bola.velocX < 0)
							bola.velocX +=1.0 ;
						else
							bola.velocX -=1.2 ;
					}			
					
					bola.velocY = -bola.velocY;
					
					
				}
				else
				{
					if(playing == 0.0)
						bola.velocY = -bola.velocY;
					else 
						bola.velocY = bola.velocY;
				playing += 0.02;
				
				}


				//PARTE LATERAL DA BARRA
				  // DIREITA
				if( (bola.posicaoX >= barra.posicaoX+80.0 && bola.posicaoX <= barra.posicaoX+105.0) && (bola.posicaoY+12.5 >= barra.posicaoY-10.0))
					bola.velocX = -bola.velocX;
			
					//ESQUERDA
				if( (bola.posicaoX+25 >= barra.posicaoX-12.5 && bola.posicaoX+25 <= barra.posicaoX) && (bola.posicaoY+12.5 >= barra.posicaoY-10.0))
					bola.velocX = -bola.velocX;

			// --- Limities Superiores da TELA
				if(bola.posicaoY <=56)
				{
					bola.posicaoY = 60;
					bola.velocY = -bola.velocY;
						
				}	
				 

				// --- Limite Lateral da TELA
				if(bola.posicaoX < 0)
				{
						
					bola.posicaoX = 0;
					bola.velocX = -bola.velocX;
					
				}
			

				if(bola.posicaoX+25 > ScreenW)
				{
						
					bola.posicaoX = ScreenW -30;
					bola.velocX = -bola.velocX;
					
				}
				
				
}
END_OF_FUNCTION(detectarColisoes);

void salvar()
{
	if(vidas >= 0)
		pontoFinal += (vidas *100);
	
	FILE *arqScore = fopen("score.txt","a");
	char c;
	char fileName[] = "score.txt";
	int i = 0;

	// SE O ARQUIVO DE RECORDES TIVER MAIS DO QUE 10 PONTOS ELE E ZERADO ----
			while( (c = fgetc(arqScore)) != EOF)
				i++;
			
			if(1)
				{
					fclose(arqScore);
					if(!remove(fileName))
						FILE *arqScore= fopen("score.txt","a");
				
			}
			

	//---------------------------------------------------------------
	if(arqScore)
		fprintf(arqScore, "*%d\n", pontoFinal,i);
	fclose(arqScore);
}
	
END_OF_FUNCTION(salvar);

void lerScore(BITMAP *Buffer)
{
	FONT *font_menuScore = load_font("fonts/font_score.pcx",NULL,NULL);
	FILE *arqScore = fopen("score.txt","r");
	char *pontos, string[8];
	int i = (rand()%5)+1;	// GERADOR DE CORES

	char c;

				if(arqScore != NULL)
				{
					int mudarY = 0, mudarX=0;
					while( (c = fgetc(arqScore)) != EOF)
					{
						if(c == '\n' )
							continue;
						if(c == '*' || c== '^')
						{
							mudarY += 30;
							mudarX=0;
						}
						else
						{
							switch(i)
							{
								//ALTERNA CORES DOS PONTOS
								//CORES:
								//   1 orange 255,172,87
								//   2 green 66,159,89
								//   3 red 255,0,0
								//   4 yellow 255,201,14
								//   5 blue 18,0,255
								case 1:
									textprintf_ex(Buffer,font_menuScore,(360+mudarX),(160+mudarY),makecol(255,172,87),-1,"%c",c);
									break;
								case 2:
									textprintf_ex(Buffer,font_menuScore,(360+mudarX),(160+mudarY),makecol(66,159,89),-1,"%c",c);
									break;
								case 3:
									textprintf_ex(Buffer,font_menuScore,(360+mudarX),(160+mudarY),makecol(255,0,0),-1,"%c",c);
									break;
								case 4:
									textprintf_ex(Buffer,font_menuScore,(360+mudarX),(160+mudarY),makecol(255,201,14),-1,"%c",c);
									break;
								case 5:
									textprintf_ex(Buffer,font_menuScore,(360+mudarX),(160+mudarY),makecol(18,0,255),-1,"%c",c);
									
									break;
							}
							rest(5);
							i = (rand()%5)+1;
						}
					
					mudarX += 16;
					}
				}
	fclose(arqScore);

	destroy_font(font_menuScore);

}
END_OF_FUNCTION(lerScore);

   // ----- Funcao Para para poder fechar janela clicando no X
void CloseButtonCallback(){closewindow = 1;}
END_OF_FUNCTION(CloseButtonCallback);

int keyrel(int k)				
{
    static int pressed = 0;
    static int keyp[KEY_MAX];
 
    if(!pressed)
    {
        /* Torna falso (0) o valor do identificador keyp (tecla pressionada)*/
        int i;
        for(i = 0; i < KEY_MAX; i++) keyp[i] = 0;
        pressed = 1;
    }
 
    /* Checando
       Checar se a tecla foi pressionada
    */
    if(key[k] && !keyp[k])
    {
        /* Torna o identificador verdadeiro (1) e retorna */
        keyp[k] = 1;
        return 0;
    }
    else if(!key[k] && keyp[k])
    {
        /* A tecla deixou de ser pressionada */
        keyp[k] = 0;
        return 1;
    }
    /* Se nada acontecer, retorna 0 */
    return 0;
}
END_OF_FUNCTION(keyrel);