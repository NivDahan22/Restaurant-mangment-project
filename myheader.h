#define _CRT_SECURE_NO_WARNINGS
#ifndef _myheader
#define _myheader
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Kitchen Products struct
typedef struct Kitchen
{
	char* ProductName;
	int Price;
	int Quantity;
	int MaxQuantity;
	struct Kitchen* next;
}Kitchen, * P_kitchen;

//Order
typedef struct Order
{
	char* ProductName;
	int Quantity;
	int Price;	
	struct Order* next;
	struct Order* prev;

}Order, * P_Order;

//Table
typedef struct Table
{
	int TableNumber;
	int bill;
	P_Order orders;

}Table, * P_Table;

void CreateProducts(FILE* out,FILE* in, P_kitchen* p);
void AddItems(FILE* out,char* prod, int q, P_kitchen* ptr);
void Error_Msg(FILE* out,char* msg);
void OrderItem(FILE* out,int TableNumber, char* ProductName, int Quantity, int MaxTablesNum, P_Table array, P_kitchen* p);
void RemoveItem(FILE* out,int TableNumber, char* ProductName, int Quantity, int MaxTablesNum, P_Table array);
void RemoveTable(FILE* out, int TableNumber, int MaxTablesNum, P_Table array, P_kitchen* k);


#endif



