#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <ctype.h>
#define MAX_ARP 10
#define BUFFER_SIZE 255

typedef int IP_add[4];
typedef int MAC_add[6];
typedef struct {
	IP_add ip_add;
	MAC_add mac_add;
	char type;
}ARP;

IP_add HOTSPOT_IP = { 192, 168, 137, 1 };
ARP ARP_LIST[MAX_ARP];

void UPDATE_INTERRUPT();
void update_arp(ARP arp);
void make_arp(char* buf, ARP* arp);
int find_ip(char* buf, IP_add ip);
int find_mac(char* buf, MAC_add mac);
int find_type(char* buf, char *type);
void print_arp(ARP arp);

int main()
{
	// First, set all objects in the ARP_LIST to null.
	for (int i = 0; i < MAX_ARP; i++)
		ARP_LIST[i].type = 'n'; // 'n' is null

	char buf[BUFFER_SIZE] = { 0, };
	ARP arp;
	IP_add ip;
	int c = 0;
	bool find = false;

	FILE* fp = _popen("arp -a", "rt");
	
	while (1) 
	{
		fp = _popen("arp -a", "rt");
		while (fgets(buf, BUFFER_SIZE, fp))
		{
			if (strlen(buf) == 1)
			{
				fgets(buf, BUFFER_SIZE, fp);

				find_ip(buf, ip);
				if (memcmp(ip, HOTSPOT_IP, 4 * sizeof(int)) == 0) // When the interface IP address matches the hotspot address
				{
					//printf("Mobile Hotspot has been detected.\n");
					find = true;

					// Read one line at a time to make ARP.
					while (fgets(buf, BUFFER_SIZE, fp))
					{
						if (strlen(buf) == 1)
							break;

						make_arp(buf, &arp);
						update_arp(arp); //After make ARP, the change is updated in the ARP_LIST.
					}

					break;
				}
			}
		}
		Sleep(1000);
	}

	_pclose(fp);


	return 0;
}

void UPDATE_INTERRUPT(ARP arp)
{
	printf("\nNew device connected to network.\n");
	print_arp(arp);
}

void update_arp(ARP arp)
{
	if (arp.type != 'n') 
	{
		for (int i = 0; i < MAX_ARP; i++)
		{
			if (ARP_LIST[i].type == 'n') {
				// Make Interrupt when new ARP has been updated. (Temporarily using a function)
				UPDATE_INTERRUPT(arp);
				ARP_LIST[i] = arp;
				break;
			}
			if (memcmp(arp.mac_add, ARP_LIST[i].mac_add, sizeof(MAC_add)) == 0)
				// Compare MAC addresses to check ARP already exists or not.
				break;
		}
	}
}

void make_arp(char* buf, ARP* arp)
{
	int r1, r2, r3;
	
	r1 = find_ip(buf, arp->ip_add);
	r2 = find_mac(buf + r1, arp->mac_add);
	r3 = find_type(buf + r1 + r2, &arp->type);
}

int find_ip(char* buf, IP_add ip)
{
	int i = 0, j = 0;
	char c;
	for (i = 0; i < strlen(buf); i++)
	{
		c = buf[i];
		if (c > '0' && c < '9')
		{
			for (j = i; j < strlen(buf); j++)
			{
				c = buf[j];
				if (c == ' ')
					break;
			}
			
			break;
		}
	}

	char ipstring[20];
	strncpy_s(ipstring, 20, buf + i, j - i);
	sscanf_s(ipstring, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);


	return j;
}

int find_mac(char* buf, MAC_add mac)
{
	int i = 0, j = 0;
	char c;
	for (i = 0; i < strlen(buf); i++)
	{
		c = buf[i];
		if ((c >= '0' && c <= '9')
			|| (c >= 'a' && c <= 'f'))
		{
			for (j = i; j < strlen(buf); j++)
			{
				c = buf[j];
				if (c == ' ')
					break;
			}

			break;
		}
	}

	char macstring[20];
	strncpy_s(macstring, 20, buf + i, j - i);
	sscanf_s(macstring, "%x-%x-%x-%x-%x-%x", &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
	

	return j;
}

int find_type(char* buf, char* type)
{
	int i = 0, j = 0;

	for (i = 0; i < strlen(buf); i++) 
	{
		if (buf[i] != ' ') 
		{
			for (j = i; j < strlen(buf); j++)
				if (buf[j] == ' ')
					break;
			break;
		}
	}
		
	char typestring[8];
	strncpy_s(typestring, 8, buf + i, j - i);
	if (strcmp(typestring, "정적") == 0)
		*type = 's';
	else if (strcmp(typestring, "동적") == 0)
		*type = 'd';
	else
		*type = 'n';


	return 0;
}

void print_arp(ARP arp)
{
	if (arp.type != 'n') 
	{
		int i = 0;

		printf(" IP Address: ");
		for (i = 0; i < 3; i++)
			printf("%d.", arp.ip_add[i]);
		printf("%d\n", arp.ip_add[3]);

		printf(" MAC Address: ");
		for (i = 0; i < 5; i++)
			printf("%02X-", arp.mac_add[i]);
		printf("%02X\n", arp.mac_add[5]);

		printf(" Type: %c", arp.type);
		printf("\n");
	}
}