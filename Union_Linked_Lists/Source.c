#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "md5.h" //using void md5String(const char* input, char* output);
#include "md4.h" //using void md4String(const char* input, char* output);

#define MD4_HASH 0
#define MD5_HASH 1
#define True 1
#define False 0
typedef unsigned char HashType;
typedef unsigned char Byte;
typedef char Bool;

typedef struct msgWithHash
{
	unsigned int id;
	char* text;
	union
	{
		Byte md4[16];
		Byte md5[16];
	} hashDigest;
	HashType type;
}msgWithHash;

typedef struct msgListNode msgListNode;

struct msgListNode
{
	msgWithHash* data;
	msgListNode* next;
};

void print_hash(const unsigned char* p);  //Done
msgWithHash* createMessage(unsigned int id, const char* text, HashType type); //Done
void printMessage(const msgWithHash* message); //Done
msgListNode* addMessage(msgListNode* head, msgWithHash* data); //Done
int findMsgByHashIterative(const msgListNode* head, Byte hash[16]); //Done
int findMsgByHashRecursive(const msgListNode* head, Byte hash[16]); //Done
msgListNode* deleteMessage(msgListNode* head, unsigned int id); //Done
Bool verifyIntegrity(msgListNode* head, unsigned int id, Byte compareHash[16]); //Done
msgListNode* reverseMsgList(msgListNode* head);  //Done
void freeMsgList(msgListNode* head);  //Done
void printlist(msgListNode* head);

int main() {
	int result = 0;
	msgListNode* head = NULL;
	Byte* hash1 = NULL, * hash2 = NULL, * hash3 = NULL;
	Bool integ_result = 0;

	const msgWithHash* msg1 = createMessage(87651233, "", MD4_HASH);
	const msgWithHash* msg2 = createMessage(3, "banana1", MD5_HASH);
	const msgWithHash* msg3 = createMessage(275, "banana2!", MD4_HASH);
	const msgWithHash* msg4 = createMessage(342234, "Hello World", MD4_HASH);
	const msgWithHash* msg5 = createMessage(685, "Hi", MD5_HASH);

	printMessage(msg1);
	printMessage(msg2);
	printMessage(msg3);
	printMessage(msg4);

	head = addMessage(head, msg3);
	head = addMessage(head, msg2);
	head = addMessage(head, msg4);
	head = addMessage(head, msg1);
	printf("\nMessages List:\n--------------------------------------------\n");
	printlist(head);


	head = reverseMsgList(head);
	printf("\nReverse Messages List:\n--------------------------------------------\n");
	printlist(head);

	hash1 = msg3->hashDigest.md4;
	hash2 = msg2->hashDigest.md5;
	hash3 = msg5->hashDigest.md5;


	result = findMsgByHashIterative(head, hash1);
	if (result == -1)
		printf("\nfindMsgByHashIterative - Message was not found\n");
	else if (result == -2)
		printf("\nfindMsgByHashIterative - Error was accure\n");
	else
		printf("\nfindMsgByHashIterative - Message was found - message index is: %d\n", result);


	result = findMsgByHashRecursive(head, hash1);
	if (result == -1)
		printf("\nfindMsgByHashRecursive - Message was not found\n");
	else if (result == -2)
		printf("\nfindMsgByHashRecursive - Error was accure\n");
	else
		printf("\nfindMsgByHashRecursive - Message was found - message index is: %d\n", result);


	result = findMsgByHashRecursive(head, hash3);
	if (result == -1)
		printf("\nfindMsgByHashRecursive - Message was not found\n");
	else if (result == -2)
		printf("\nfindMsgByHashRecursive - Error was accure\n");
	else
		printf("\nfindMsgByHashRecursive - Message was found - message index is: %d\n", result);


	integ_result = verifyIntegrity(head, 275, hash2);
	if (integ_result ==  -1)
		printf("\nverifyIntegrity - Message was not found\n");
	else if (integ_result == 0)
		printf("\nverifyIntegrity - Message was found, but Hash is not match\n");
	else
		printf("\nverifyIntegrity - Message was found, and Hash is matched - Verify pass\n");


	integ_result = verifyIntegrity(head, 275, hash1);
	if (integ_result == -1)
		printf("\nverifyIntegrity - Message was not found\n");
	else if (integ_result == 0)
		printf("\nverifyIntegrity - Message was found, but Hash is not match\n");
	else
		printf("\nverifyIntegrity - Message was found, and Hash is matched - Verify pass\n");


	integ_result = verifyIntegrity(head, 685, hash3);
	if (integ_result == -1)
		printf("\nverifyIntegrity - Message was not found\n");
	else if (integ_result == 0)
		printf("\nverifyIntegrity - Message was found, but Hash is not match\n");
	else 
		printf("\nverifyIntegrity - Message was found, and Hash is matched - Verify pass\n");


	head = deleteMessage(head, 342234);
	printf("\nMessages List (after delete message):\n--------------------------------------------\n");
	printlist(head);

	freeMsgList(head);
}

void print_hash(const unsigned char* p) {
	for (unsigned int i = 0; i < 16; ++i) {
		printf("%02x", p[i]);
	}
	printf("\n");
}

msgWithHash* createMessage(unsigned int id, const char* text, HashType type) {
	Byte temp_hash[16] = { 0 };
	msgWithHash* new_msg = malloc(sizeof(msgWithHash));
	if (!new_msg) { puts("allocation failed"); exit(0); }
	new_msg->text = (char*)malloc(strlen(text) + 1);
	if (!new_msg->text) { puts("allocation failed"); exit(0); }
	new_msg->id = id;
	strcpy(new_msg->text, text);
	new_msg->type = type;
	if (new_msg->type == MD4_HASH) {
		md4String(new_msg->text, new_msg->hashDigest.md4);
		return new_msg;
	}
	else if (new_msg->type == MD5_HASH) {
		md5String(new_msg->text, new_msg->hashDigest.md5);
		return new_msg;
	}
	else
		return NULL;
}

void printMessage(const msgWithHash* message) {
	printf("Message ID:  %d\n", message->id);
	printf("Message Test:  %s\n", message->text);
	if (message->type == MD4_HASH) {
		printf("Message Hash MD4:  ");
		print_hash(message->hashDigest.md4);
	}
	else {
		printf("Message Hash MD5:  ");
		print_hash(message->hashDigest.md5);
	}
	printf("\n");
}

msgListNode* addMessage(msgListNode* head, msgWithHash* data) {
	msgListNode* tmp = head;
	msgListNode* new_msg_add = malloc(sizeof(msgListNode));
	if (!new_msg_add) { puts("allocation failed"); exit(0); }
	new_msg_add->data = data;
	new_msg_add->next = NULL;
	if (head == NULL || new_msg_add->data->id < tmp->data->id) {
		new_msg_add->next = head;
		head = new_msg_add;
	}
	else
	{
		while (tmp->next != NULL && new_msg_add->data->id > tmp->next->data->id)
			tmp = tmp->next;
		new_msg_add->next = tmp->next;
		tmp->next = new_msg_add;
	}
	return head;
}

int findMsgByHashIterative(const msgListNode* head, Byte hash[16]) {
	msgListNode* tmp = head;
	int count = 0, index = 1;
	if (hash == NULL)
		return -2;
	while (tmp != NULL) {
		for (int i = 0; i < 16; i++)
		{
			if ((tmp->data->hashDigest.md4[i]) != hash[i])
				break;
			else
				count++;
		}
		if (count == 16)
			return index;
		index++;
		tmp = tmp->next;
	}
	return -1;
}

int findMsgByHashRecursive(const msgListNode* head, Byte hash[16]) {
	msgListNode* tmp = head;
	int count = 0, result = 0;;
	if (hash == NULL)
		return -2;
	if (head == NULL)
		return -1;
	else {
		for (int i = 0; i < 16; i++) {
			{
				if ((tmp->data->hashDigest.md4)[i] != hash[i])
					break;
				else
					count++;
			}
		}
		if (count == 16)
			return 1;
		result = findMsgByHashRecursive(head->next, hash);
		if (result == -1)
			return -1;
		else if (result == -2)
			return -2;
		else
			return 1 + result;
	}

}

msgListNode* deleteMessage(msgListNode* head, unsigned int id) {
	msgListNode* tmp = head, * tmp2 = NULL;
	if (head->data->id == id) {
		head = head->next;
		free(tmp->data->text);
		free(tmp);
	}
	else {
		while (tmp->next != NULL && tmp->data->id != id) {
			tmp2 = tmp;
			tmp = tmp->next;
		}
		if (tmp->next == NULL && tmp->data->id != id)
			printf("\nSorry, we didn't found message to delete\n");
		else if (tmp->next == NULL && tmp->data->id == id)
		{
			tmp2->next = NULL;
			free(tmp->data->text);
			free(tmp);
		}
		else
		{
			tmp2->next = tmp->next;
			free(tmp->data->text);
			free(tmp);
		}
	}
	return head;
}

Bool verifyIntegrity(msgListNode* head, unsigned int id, Byte compareHash[16]) {
	msgListNode* tmp = head;
	int count = 0;
	while (tmp != NULL && tmp->data->id != id)
		tmp = tmp->next;
	if (tmp == NULL)
		return -1;
	else {
		for (int i = 0; i < 16; i++) {
			{
				if ((tmp->data->hashDigest.md4)[i] != compareHash[i])
					break;
				else
					count++;
			}
		}
		if (count == 16)
			return 1;
		else
			return 0;
	}
}

msgListNode* reverseMsgList(msgListNode* head) {
	msgListNode* tmp1 = head, * tmp2 = NULL;
	if (head->next == NULL)
		return head;
	tmp2 = tmp1->next;
	head = tmp2->next;
	if (head->next == NULL)
	{
		tmp1->next = NULL;
		tmp2->next = tmp1;
		head->next = tmp2;
	}
	else {
		tmp1->next = NULL;
		while (head->next != NULL)
		{
			tmp2->next = tmp1;
			tmp1 = tmp2;
			tmp2 = head;
			head = head->next;
		}
		tmp2->next = tmp1;
		head->next = tmp2;
	}
	return head;
}

void freeMsgList(msgListNode* head) {
	msgListNode* tmp = head;
	while (head != NULL) {
		head = head->next;
		free(tmp->data->text);
		free(tmp);
		tmp = head;
	}
}

void printlist(msgListNode* head) {
	msgListNode* tmp = head;
	while (tmp != NULL) {
		printMessage(tmp->data);
		tmp = tmp->next;
	}
}