#include <stdio.h>
#include <stdlib.h>
#define MAX_LENGTH 100
#define NB_ROWS 4
#define NB_COLUMNS 4
#define MAX_VALUE 5
#define EMPTY 0
#define INF 999999
int NB_BLOCK = 8;
//Khai bao cau truc List
typedef struct List {
   int data[MAX_LENGTH];
   int size;
} List;
//Khoi tao danh sach rong
void makeNULLList(List *list) {
   list->size = 0;
}
//Kiem tra danh sach co rong khong
int emptyList(List list) {
   return list.size == EMPTY;
}
//Kiem tra danh sach co day khong
int fullList(List list) {
   return list.size == MAX_LENGTH;
}
//ElementAt
int elementAt(List list, int p) {
   return list.data[p - 1];
}
//Them vao danh sach
void pushList(List *list, int n) {
   list->data[list->size] = n;
   list->size++;
}
//Khai bao cau truc Coord
typedef struct Coord {
   int x, y;
}Coord;
//Khai bao cau truc ListCoord
typedef struct ListCoord {
   Coord data[MAX_LENGTH];
   int size;
}ListCoord;
//initListCoord
void initListCoord(ListCoord *list) {
   list->size = 0;
}
//appendListCoord
void appendListCoord(ListCoord *list, Coord coord) {
   list->data[list->size++] = coord;
}
//Khai bao cau truc Constrains
typedef struct Constrains {
   int data[NB_ROWS * NB_COLUMNS][NB_ROWS * NB_COLUMNS];
   int n;
}Constrains;
//initConstrains
void initConstrains(Constrains *constrains) {
   int i, j;
   for (i = 0; i < NB_ROWS * NB_COLUMNS; i++)
      for (j = 0; j < NB_ROWS * NB_COLUMNS; j++)
         constrains->data[i][j] = 0;
   constrains->n = NB_ROWS * NB_COLUMNS;
}
//indexOf
int indexOf(Coord coord) {
   return NB_ROWS * coord.x + coord.y;
}
//positionOfVertex
Coord positionOfVertex(int vertex) {
   Coord coord;
   coord.x = vertex / NB_ROWS;
   coord.y = vertex % NB_COLUMNS;
   return coord;
}
//addConstrains
int addConstrain(Constrains *constrains, Coord source, Coord target) {
   int u = indexOf(source);
   int v = indexOf(target);
   if(constrains->data[u][v] == 0) {
      constrains->data[u][v] = 1;
      constrains->data[v][u] = 1;
      return 1;
   }
   return 0;
}
//getConstrains
ListCoord getConstrains(Constrains constrains, Coord coord) {
   int i;
   int v = indexOf(coord);
   ListCoord result;
   initListCoord(&result);
   for (i = 0; i < constrains.n; i++) {
      if (constrains.data[v][i] == 1) {
         appendListCoord(&result, positionOfVertex(i));
      }
   }
   return result;
}
//Khai bao cau truc Block
typedef struct Block {
   int value;
   int currentValue;
   char operators;
   ListCoord list;
   int filledCells;
}Block;
//initBLock
void initBlock(Block *block, int sum, char c, ListCoord list) {
   block->value = sum;
   block->currentValue = 0;
   block->filledCells = 0;
   block->operators= c;
   int i;
   initListCoord(&block->list);
   for(i = 0; i < list.size; i++)
      appendListCoord(&block->list, list.data[i]);
}
//whichBLockAreYouIn
int whichBlockAreYouIn(Coord position, Block block[]) {
   int i, j;
   for (i = 0; i < NB_BLOCK; i++) {
      for (j = 0; j < block[i].list.size; j++) {
         if (position.x == block[i].list.data[j].x && position.y == block[i].list.data[j].y)
            return i;
      }
   }
}
//blockIsFilled
int blockIsFilled(Block block) {
   return block.filledCells == block.list.size;
}
//Khai bao cau truc KenKen
typedef struct KenKen {
   int cells[NB_ROWS][NB_COLUMNS];
   Constrains constrains;
   Block block[10];
}KenKen;
//initKenKen
void initKenKen(KenKen *kenken) {
   int i, j;
   for (i = 0; i < NB_ROWS; i++)
      for (j = 0; j < NB_COLUMNS; j++)
         kenken->cells[i][j] = EMPTY;
   initConstrains(&kenken->constrains);
}
//In KenKen
void printKenKen(KenKen kenken) {
   int i, j;
   printf("KenKen:\n");
   for (i = 0; i < NB_ROWS; i++) {
      printf("-------------------------\n");
      for (j = 0; j < NB_COLUMNS; j++) {
         printf("|%3d  ", kenken.cells[i][j]);
      }
      printf("|\n");
   }
   printf("------------------------\n");
   printf("KenKen Operator:\n");
   for (i = 0; i < NB_ROWS; i++) {
      printf("-----------------\n");
      for (j = 0; j < NB_COLUMNS; j++) {
         int k = whichBlockAreYouIn((Coord){i, j}, kenken.block);
         if (kenken.block[k].operators== '.')
            printf("|%3d  ", kenken.block[k].value);
         else
            printf("|%3d%c ", kenken.block[k].value, kenken.block[k].operators);
      }
      printf("|\n");
   }
   printf("-------------------------\n");
}
//isFilledKenKen
int isFilledKenKen(KenKen kenken) {
   int i, j;
   for (i = 0; i < NB_ROWS; i++)
      for (j = 0; j < NB_COLUMNS; j++)
         if (kenken.cells[i][j] == EMPTY)
            return 0;
   return 1;
}
//spreadConstrainsForm
void spreadConstrainsForm(Coord position, Constrains *constrains){
   int row = position.x, column = position.y;
   int i, j;
   for (i = 0; i < NB_ROWS; i++) {
      if (i != row) {
         Coord pos = {i, column};
         addConstrain(constrains, position, pos);
      }
   }
   for (i = 0; i < NB_COLUMNS; i++) {
      if (i != column) {
         Coord pos = {row, i};
         addConstrain(constrains, position, pos);
      }
   }
}
//resetConstrains
void resetConstrains(Constrains *constrains, int cells[NB_ROWS][NB_COLUMNS]) {
   int i, j;
   for (i = 0; i < NB_ROWS; i++)
      for (j = 0; j < NB_COLUMNS; j++)
         constrains->data[i][j] = 0;
   for(i = 0; i < NB_ROWS; i++)
      for (j = 0; j < NB_COLUMNS; j++)
         if (cells[i][j] != 0)
            spreadConstrainsForm((Coord){i, j}, constrains);
}
//getAvailableValues
List getAvailableValues(Coord position, KenKen kenken) {
   ListCoord posList = getConstrains(kenken.constrains, position);
   int availables[MAX_VALUE];
   int i;
   for (i = 1; i < MAX_VALUE; i++)
      availables[i] = 1;
   for (i = 0; i < posList.size; i++) {
      Coord pos = posList.data[i];
      if (kenken.cells[pos.x][pos.y] != EMPTY) {
         availables[kenken.cells[pos.x][pos.y]] = 0;
      }
   }
   List result;
   makeNULLList(&result);
   for (i = 1; i < MAX_VALUE; i++) {
      if (availables[i])
         pushList(&result, i);
   }
   return result;
}
//getNextEmptyCell
Coord getNextEmptyCell(KenKen kenken) {
   int i, j;
   for (i = 0; i < NB_ROWS; i++) {
      for (j = 0; j < NB_COLUMNS; j++) {
         Coord pos = {i, j};
         if (kenken.cells[i][j] == EMPTY)
            return pos;
      }
   }
}
int resultFromOperatorAndTwoValues(int a, int b, char c) {
   if(c == '+')
      return a + b;
   if(c == '-')
   if(a > b)
      return a - b;
   else
      return b - a;
   if(c == '/')
   if(a > b)
      return a / b;
   else
      return b / a;
   if(c == '*' || c == 'x')
      return a * b;
   return b;
}
//getnextMinDomainCell
Coord getNextMinDomainCell(KenKen kenken){
   int minLength = INF;
   int i, j;
   Coord result;
   for (i = 0; i < NB_ROWS; i++){
      for (j = 0; j < NB_COLUMNS; j++){
         if (kenken.cells[i][j] == EMPTY){
            Coord pos = {i, j};
            int availablesLength = getAvailableValues(pos, kenken).size;
            if (availablesLength < minLength){
               minLength = availablesLength;
               result = pos;
            }
         }
      }
   }
   return result;
}
int exploredCounter = 0;
int kenkenBackTracking(KenKen *kenken) {
   if (isFilledKenKen(*kenken))
      return 1;
   Coord position = getNextEmptyCell(*kenken);
   // Coord position = getNextMinDomainCell(*kenken);
   List availables = getAvailableValues(position, *kenken);
   if (availables.size == 0) {
      return 0;
   }
   int j;
   for (j = 0; j < availables.size; j++) {
      int value = availables.data[j];
      int blockNB = whichBlockAreYouIn(position, kenken->block);
      kenken->cells[position.x][position.y] = value;
      int currentValue = kenken->block[blockNB].currentValue;
      if (kenken->block[blockNB].filledCells == 0) {
         kenken->block[blockNB].filledCells++;
         kenken->block[blockNB].currentValue += value;
         spreadConstrainsForm(position, &kenken->constrains);
      }else{
         kenken->block[blockNB].filledCells++;
         kenken->block[blockNB].currentValue = resultFromOperatorAndTwoValues(kenken->block[blockNB].currentValue, value, kenken->block[blockNB].operators);
         spreadConstrainsForm(position, &kenken->constrains);
      }
      if(blockIsFilled(kenken->block[blockNB]) && (kenken->block[blockNB].currentValue != kenken->block[blockNB].value)) {
         kenken->cells[position.x][position.y] = EMPTY;
         kenken->block[blockNB].filledCells--;
         kenken->block[blockNB].currentValue = currentValue;
         resetConstrains(&kenken->constrains, kenken->cells);
         continue;
         // return 0;
      }
      exploredCounter++;
      if(kenkenBackTracking(kenken))
         return 1;
      kenken->cells[position.x][position.y] = EMPTY;
      kenken->block[blockNB].filledCells--;
      kenken->block[blockNB].currentValue = currentValue;
      resetConstrains(&kenken->constrains, kenken->cells);
   }
   return 0;
}
int solveKenKen(KenKen *kenken) {
    initConstrains(&kenken->constrains);
    exploredCounter = 0;
    return kenkenBackTracking(kenken);
}
//readKenKen
void readKenKen(KenKen *kenken) {
   int i;
   printf("Number of Cage: ");
   scanf("%d", &NB_BLOCK);
   for(i = 0; i < NB_BLOCK; i++) {
      ListCoord cage;
      initListCoord(&cage);
      int k, n, value;
      // printf("Enter value of cage: ");
      scanf("%d", &value);
      char c;
      // printf("Enter operator: ");
      scanf(" %c", &c);
      getchar();
      // printf("Number of cells of cage%d: ", i + 1);
      scanf("%d", &n);
      for (k = 0; k < n; k++) {
         // printf("Enter position(x,y): ");
         int u, v;
         scanf("%d%d", &u, &v);
         appendListCoord(&cage, (Coord){u, v});
      }
      initBlock(&kenken->block[i], value, c, cage);
   }
}
//Main
int main() {
   KenKen kenken;
   initKenKen(&kenken);
   int i;
   freopen("test4.txt", "r", stdin);
   readKenKen(&kenken);
   if (solveKenKen(&kenken)) {
      printf("Solved\n");
      printKenKen(kenken);
   }else
      printf("Can not solve\n");
   printf("Explored %d states\n", exploredCounter);
}
