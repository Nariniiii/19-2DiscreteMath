#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv){
  char buffer[51][51];
  int* labels[2]; //레이블 저장; [0]가로, [1]세로 레이블
  int** num; //a1/1부터 aM/N까지 값을 저장
  int N = 0, M = 0;
  int i = 0, j = 0, k = 0;
  int sum = 0;
  char* splitPtr;


  if(argc == 1) {
    printf("Please enter the input file name togeter.\n");
    return -1;
  }
  // stdin to buffer
  FILE* fin = fopen(argv[1], "r");

  fgets(buffer[0], sizeof(buffer), fin);
  splitPtr = strtok(buffer[0], " ");
  while(splitPtr != NULL){
    N++;
    splitPtr = strtok(NULL, " ");
  }

  i = 1;
  while(1){
    fgets(buffer[i], sizeof(buffer), fin);
    if(feof(fin)) break;
    i++;
    M++;
  }

  labels[0] = (int*) malloc(sizeof(int) * N);
  labels[1] = (int*) malloc(sizeof(int) * M);
  num = (int**) malloc(sizeof(int*) * M+1);
  for(i=1; i<=M; i++){
    num[i] = (int*) malloc(sizeof(int) * N+1);
  }

  fseek(fin, 0l, SEEK_SET);
  for(i=0; i<N; i++){
    fscanf(fin, "%d", &labels[0][i]);
  }

  for(i = 1; i<=M; i++){
    for(j = 1; j<=N; j++){
      fscanf(fin, "%d", &num[i][j]);
    }
    fscanf(fin, "%d", &labels[1][k]);
    k++;
  }

  for(i = 1; i<=M; i++){
    sum = 0;
    for(j = 1; j<=N; i++){
      sum += num[i][j];
    }
    labels[1][j-1] = sum - labels[1][j-1];
  }

  fclose(fin);

  //////////////// making formula //////////////////
  FILE* fp = fopen("formula", "w");
  int x, y, n; // cols, row

  // declaration a(row/col)
  for (y = 1 ; y <= M ; y++)
  	for (x = 1 ; x <= N ; x++)
  		fprintf(fp, "(declare-const a%d_%d Int)\n", y, x);

  //  Q1: All numbers in max spaces are between 0 to 1.
  for (y = 1 ; y <= M ; y++)
  	for (x = 1 ; x <= N ; x++)
      fprintf(fp, "(assert (or (= a%d_%d 1) (= 0 a%d_%d)))\n", y, x, y, x);


  //  Q2: 가로 줄 같은 지 확인
  for (y = 1 ; y <= M ; y++){
    fprintf(fp, "(assert ");
    fprintf(fp, "(= ");
    for(i=1; i<=N-1; i++) fprintf(fp, "(+ ");
  	for (x = 1 ; x <= N ; x++){
      fprintf(fp, "(* a%d_%d %d)", y, x, num[y][x]);
      if(x != 1) fprintf(fp, ") ");
    }
    fprintf(fp, "%d)", labels[1][y-1]);
    fprintf(fp, ")\n");
  }


  //  Q3: 세로 줄 같은 지 확인
  for (x = 1 ; x <= N ; x++){
    fprintf(fp, "(assert ");
    fprintf(fp, "(= ");
    for(i=1; i<=N-1; i++) fprintf(fp, "(+ ");
  	for (y = 1 ; y <= M ; y++){
      fprintf(fp, "(* a%d_%d %d)", y, x, num[y][x]);
      if(y != 1) fprintf(fp, ") ");
    }
    fprintf(fp, "%d)", labels[0][x-1]);
    fprintf(fp, ")\n");
  }


  // get model
  fprintf(fp, "(check-sat)\n(get-model)\n");
	fclose(fp);

///////////print result
  fin = popen("z3 formula", "r");
  char buf[128];
  char b[128];
  char s[128];
  char t[128];

  int** board = (int**) malloc(sizeof(int*) * M);
  for(i = 0; i < M; i++) {
    board[i] = (int*) malloc(sizeof(int) * N);
  }

  fscanf(fin, "%s", buf);
  if(strcmp(buf, "unsat") == 0) printf("No solution\n");
  else {
    fscanf(fin, "%s", buf);

    while(!feof(fin)){
      fscanf(fin, "%s %s %s %s %s", b, s, b, b, t);
      sscanf(s, "a%d_%d", &i, &j);
      //if(t[0] - '0' == 0) board[i-1][j-1] = 1;
      //else if(t[0] - '0' == 1) board[i-1][j-1] = 0;
      board[i-1][j-1] = t[0] - '0';
    }
    fclose(fin);

    for(i=0; i<M; i++){
      for(j=0; j<N; j++) printf("%d ", board[i][j]);
      printf("\n");
    }
  }


  for(i = 0; i <= M; i++) free(num[i]);
  free(num);
  free(labels[0]);
  free(labels[1]);

  for(i = 0; i < M; i++) free(board[i]);
  free(board);

  return 0;
}
