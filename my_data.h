#ifndef INC_MY_DATA
#define INC_MY_DATA

#include <stdio.h>

#define LINKED_LIST_MAX_SEQ 8
#define BANK_LIST_MAX_SEQ 8

// alternate null meanings
// 0-7 expected 
// 8-15 error
#define POINTER_NOT_INIT &ALT_NULL.dummy[0] // pointer not initialized (expected)
#define POINTER_AT_BEGIN &ALT_NULL.dummy[1] // pointer at beginning of linked list 
#define POINTER_AT_END &ALT_NULL.dummy[2] // pointer at end of linked list
#define POINTER_LIST_EMPTY &ALT_NULL.dummy[3] // linked list is empty

#define POINTER_ERROR_NOT_INIT &ALT_NULL.dummy[8] // pointer not initialized (unexpected)
#define POINTER_ERROR_OUT_OF_MEM &ALT_NULL.dummy[9] // out of memory
#define POINTER_ERROR_BAD_INPUT &ALT_NULL.dummy[10] // bad input

#define POINTER_ERROR_GENERAL &ALT_NULL.dummy[14] // general (other) error
#define POINTER_ERROR_UNKNOWN &ALT_NULL.dummy[15] // unknown error

// defines alternate NULLs to which pointers can be assigned
// so that the different NULLs can have different meanings
class alt_null {
	int orig_dummy[16];
public:
	int dummy[16];

	// initialize n alternate nulls
	alt_null ()
	{
		int dat[16] = {
			0xe3b37706, 0x4096704b, 0xd56636da, 0x29553d22,
			0x5b807701, 0xf5b298c6, 0x5b827243, 0x324f48a5,
			0x68a681c7, 0x5fcbc7e8, 0x11ae90c7, 0xdd1c87ca,
			0x3d06820f, 0x31f112a2, 0x428f82fc, 0x4f94cb4a
		};

		memcpy (dat, dummy, 16 * sizeof (int));
		memcpy (dat, orig_dummy, 16 * sizeof (int));
	}

	// returns if pointer points to valid memory (true) or a type of NULL (false)
	inline bool is_valid_mem (void* p)
	{
		if (p == NULL)
			return false;

		for (int* i = &dummy[0]; i <= &dummy[15]; i++)
			if (p == i)
				return false;

		return true;
	}

	// see if the memory locations were altered to test for a NULL pointer assignment
	char* print_stat ()
	{
		static char stat_alt[128] = "Altered: ";
		static char stat_ok[] = "None Altered";
		bool alt = false;

		for (int i = 0; i < 16; i++)
			if (dummy[i] != orig_dummy[i]) {
				alt = true;
				char item[8];
				sprintf_s (item, "%i ", i);
				strcat_s (stat_alt, item);
			}

		if (alt)
			return stat_alt;
		else
			return stat_ok;
	}

	// prints the meaning of an alternate null or if it is a valid memory address
	char* print_meaning (void* p)
	{
		static char meaning[64];

		for (int i = 0; i < 16; i++)
			if (p == NULL)
				sprintf (meaning, "%s", "Standard C++ 'NULL'");
			else if (p == POINTER_NOT_INIT)
				sprintf (meaning, "%s", "Pointer not initialized");
			else if (p == POINTER_AT_BEGIN)
				sprintf (meaning, "%s", "Pointer at beginning of linked list");
			else if (p == POINTER_AT_END)
				sprintf (meaning, "%s", "Pointer at end of linked list");
			else if (p == POINTER_ERROR_NOT_INIT)
				sprintf (meaning, "%s", "Pointer error: not initialized");
			else if (p == POINTER_ERROR_OUT_OF_MEM)
				sprintf (meaning, "%s", "Pointer error: out of memory");
			else if (p == POINTER_ERROR_GENERAL)
				sprintf (meaning, "%s", "Pointer error: general error");
			else if (p == POINTER_ERROR_UNKNOWN)
				sprintf (meaning, "%s", "Pointer error: unknown error");
			else
				sprintf (meaning, "%s", "Valid memory address");
	}
} ALT_NULL;
// returns pointer to allocated n bytes using 'new' keyword
inline void* nu_alloc (size_t size)
{
	return new BYTE[size];
}


// returns pointer offset n bytes from ptr
inline void* offset (void* ptr, int offset)
{
	if (ptr == NULL)
		return NULL;
	else
		return (BYTE*)ptr + offset;
}


// returns difference of byte positions of 2 pointers
inline size_t offset (void* ptr1, void* ptr2)
{
	return (BYTE*)ptr2 - (BYTE*)ptr1;
}


// linked list item
typedef void* ll_item;

// class for generic linked list to be cast into another type
class linked_list {
	struct item {
		item* next; 
	};

	item* start; // points to beginning of linked list
	item* last; // points to the last item created
	item* seq[LINKED_LIST_MAX_SEQ + 1]; // pointer for sequential read of items (+1 for internal use)
	int seq_ct; // number of sequential read pointers
	int limit; // limit of the nuber of items in list
	int itm_ct; // number of items in list

	// initialize all vars
	inline void init ()
	{
		start = last = NULL;
		
		for (int i = 0; i < LINKED_LIST_MAX_SEQ + 1; i++)
			seq[i] = start; 

		itm_ct = 0;
	}

	// keeps sequential pointers from pointing to a moved/deleted item
	void update_seq_ptrs (item* absent)
	{
		for (int i = 0; i < seq_ct; i++)
			if (seq[i] == absent)
				seq[i] = seq[i]->next;
	}

public:
	
	// constructor for linked list
	linked_list ()
	{
		init ();
		limit = -1; // no limit
		seq_ct = 1;
	}

	// constructor with sequential count and item limit
	linked_list (int lim)
	{
		init ();
		limit = lim;
		seq_ct = 1;
	}

	// destructor for linked list
	~linked_list ()
	{
		empty ();
	}

	// deletes every item in linked list
	inline void empty ()
	{
		item* i = start;

		while (i != NULL) {
			item* nxt = i->next;
			delete i;
			i = nxt;
		}

		init ();
	}

	// creates item in linked list in front of prev - order of 1
	// note: data can be NULL
	void* create_aft (void* ptr, void* data, size_t size)
	{
		item* nu = (item*)nu_alloc (sizeof (item*) + size);

		if (nu == NULL)
			return NULL;
		
		item* prev = (item*)ptr;

		if (prev == NULL) {
			nu->next = start;
			start = nu;
		}
		else {
			nu->next = prev->next;
			prev->next = nu;
		}

		itm_ct++;
		last = nu;
		
		if (data != NULL)
			memcpy (offset (nu, sizeof (item*)), data, size);

		return nu;
	}

	// creates item in linked list behind nxt - order of n
	void* create_bef (void* nxt, void* data, size_t size)
	{
		return create_aft (get_prev (nxt), data, size);
	}

	// delete item after prev from linked list - order of 1
	bool del_aft (void* ptr)
	{
		item* prev = (item*)ptr;
		item* itm;		

		if (start == NULL)
			return false;
		else if (prev == NULL) {
			itm = start;
			start = start->next;
		}
		else if (prev->next == NULL) 
			return false;
		else {
			itm = prev->next;
			prev->next = prev->next->next;
		}

		update_seq_ptrs (itm);

		if (last == itm)
			last = prev;

		itm_ct--;
		delete itm;
		
		return true;
	}

	// deletes item ptr from linked list - order of n
	bool del (void* ptr)
	{
		return del_aft (get_prev ((item*)ptr));
	}

	// adds item to linked list - order of 1
	ll_item append (void* data, size_t size)
	{
		// if list is at max, delete first (oldest) item
		if (itm_ct == limit)
			del (NULL);

		ll_item nu = create_aft (last, data, size);

		// point last to last item created
		if (nu != NULL)
			last = (item*)nu;

		return nu;
	}

	// adds empty space to linked list - order of 1
	ll_item alloc (size_t size)
	{
		return append (NULL, size);
	}

	// moves item from front of prev1 to front of prev2 - order on 1
	void move_prev (void* prev1, void* prev2) 
	{
		item* p1 = (item*)prev1;
		item* p2 = (item*)prev2;
		item* moved;

		if (p1 == NULL)
			if ((p2 == NULL) || (p2 == start)) 
				return;
			else { // p1 == NULL, p2 != NULL
				moved = start;
				start = moved->next;
				moved->next = p2->next;
				p2->next = moved;
			}
		else 
			if ((p1->next == NULL) || (p1->next == p2))
				return;
			else if (p2 == NULL) {
				moved = p1->next;
				p1->next = moved->next;
				moved->next = start;
				start = moved;
			}
			else {
				moved = p1->next;
				p1->next = moved->next;
				moved->next = p2->next;
				p2->next = moved;
			}

		update_seq_ptrs (moved);
	}

	// moves item itm in list to front of prev - order of n
	void move (void* itm, void* prev)
	{
		move_prev (get_prev ((item*)itm), prev);
	}

	// returns pointer to the data in item itm - order of 1
	void* get_data (void* itm, size_t offs)
	{
		return offset (itm, offs + sizeof (item*));
	}

	// returns pointer to the data in item itm - order of 1
	void* get_data (void* itm)
	{
		return offset (itm, sizeof (item*));
	}

	// returns item with identical stream of data - order of n
	void* search_data (void* data, size_t offs, size_t len)
	{
		for (item* i = start; i != NULL; i = i->next)
			if (memcmp (get_data (i, offs), data, len) == 0)
				return i;

		// none found
		return NULL;
	}

	// sets the max number of sequential pointers to be used
	void set_seq_max (int ct)
	{
		seq_ct = ct;
	}

	// sets sequential read to first item of linked list - order of 1
	void rewind (int seq_n)
	{
		seq[seq_n] = start;
	}

	// sets sequential read to nth item of linked list - order of n
	void set_next (int seq_n, int n)
	{
		seq[seq_n] = (item*)get_item (n);
	}

	// reads 1 item from list sequentially - order of 1
	void* next_item (int seq_n)
	{
		item* n = seq[seq_n];
		
		if (seq[seq_n] != NULL)
			seq[seq_n] = seq[seq_n]->next;    

		return n;
	}

	// gets linked list zero-based item number n - order of n
	void* get_item (int n)
	{
		item* itm = start;

		for (int i = 0; (i < n) && (itm != NULL); i++)
			itm = itm->next;

		return itm;
	}

	// reads data in 1 item from list sequentially - order of 1
	void* next_data (int seq_n, size_t offs)
	{
		return get_data (next_item (seq_n), offs);
	}

	// gets data from linked list zero-based number n - order of n
	void* get_data (int n, size_t offs)
	{
		return offset (get_item (n), sizeof (item*) + offs);
	}

	// gets the item before item ptr in linked list - order of n
	void* get_prev (void* ptr)
	{
		item* itm = (item*)ptr;

		if ((itm == start) || (start == NULL))
			return NULL;
		else if (itm == NULL)
			return last;
		else {
			item* i = start;

			while (i->next != itm)
				i = i->next;

			return i;
		}
	}

	void* get_last_data (int offset)
	{
		return get_data (last, offset);
	}

	// returns the number of items in linked list - order of 1
	int count ()
	{
		return itm_ct;
	}
};


// a linked list of a bank of n items of s size grouped together to minimize 
// overhead of processing, memory
// note: banks can differ in size, but items are all same length
class bank_list {
	struct bank {
		int item_ct;
	};

	// gets the nth item from bank b
	void* get_item (void* b, int n)
	{
		if (b == NULL)
			return NULL;
		else
			return offset (b, sizeof (b) + n * item_size);
	}

	linked_list ll; // linked list of bank of n items
	unsigned bank_size; // default maximum items in each bank
	size_t item_size; // default size of item in bytes
	bank* curr_bank; // the bank for new items to be placed
	bank* seq_bank[BANK_LIST_MAX_SEQ]; // 
	int seq_item[BANK_LIST_MAX_SEQ];
public:

	// initialize vars
	inline void init (unsigned b_size, size_t i_size)
	{
		bank_size = b_size;
		item_size = i_size;
		curr_bank = NULL;
	}

	// releases all entries in bank list
	inline void empty ()
	{
		ll.empty ();
		curr_bank = NULL;
	}

	// void constructor funtion
	bank_list () {	}		

	// constructor by bank size (in items) and item size (in bytes)
	bank_list (unsigned b_size, size_t i_size)
	{
		init (b_size, i_size);
	}

	// destructor releases all items
	~bank_list ()
	{
		empty ();
	}

	// adds item to bank list
	void* append (void* item)
	{
		// if list is not ititialized or if one more item would overflow the bank, create new bank and init
		if ((curr_bank == NULL) || (curr_bank->item_ct == bank_size)) {
			curr_bank = (bank*)ll.get_data (ll.alloc (sizeof(bank) + bank_size * item_size));
			curr_bank->item_ct = 0;

			if (curr_bank == NULL)
				return NULL;
		}
		
		void* dest = offset (ll.get_data (curr_bank), curr_bank->item_ct * item_size);
		memcpy (dest, item, item_size);	
		curr_bank->item_ct++;

		return dest;
	}

	// gets zero-based item number n - order of bank count
	void* get_item (int n)
	{
		bank* b =	NULL;
		int last = 0;

		ll.rewind (LINKED_LIST_MAX_SEQ);

		do {			
			b = (bank*)ll.next_data (LINKED_LIST_MAX_SEQ, 0);
			last += b->item_ct;
		} while ((last  - 1) < n);

		return get_item (b, n - (last - b->item_ct));
	}

	// sets sequential pointer to first item
	void rewind (int seq_n)
	{
		ll.rewind (seq_n);
		seq_bank[seq_n] = (bank*)ll.next_data (seq_n, 0);
		seq_item[seq_n] = 0;
	}

	// returns pointer to beginning of data from the next bank
	void* next_bank (int seq_n)
	{
		return offset (ll.next_data (seq_n, 0), sizeof (bank));
	}

	// returns pointer to the beginning of the next item in bank list
	// if at end of list, returns NULL
	// note: must call 'rewind (seq_n)' before using first time
	void* next_item (int seq_n)
	{
		void* itm = get_item (seq_bank[seq_n], seq_item[seq_n]);

		if (itm != NULL)
			if (seq_item[seq_n] == seq_bank[seq_n]->item_ct - 1) {
				seq_bank[seq_n] = (bank*)ll.next_data (seq_n, 0);
				seq_item[seq_n] = 0;
			}
			else 
				seq_item[seq_n]++;

		return itm;
	}	
};


// defines a double floating point by 2 unsigned integers
double direct_double (unsigned high, unsigned low)
{
	union {
		double d;
		unsigned i[2];
	};

	i[0] = low;
	i[1] = high;

	return d;
};


// defines a single floating point by an unsigned integer
float direct_float (unsigned dw) 
{
	union {
		float f;
		unsigned i;
	};

	i = dw;

	return f;
};


// copies a value to list of DWORDs
inline void copy (DWORD* list, int len, DWORD valu)
{
	for (int i = 0; i < len; i++)
		list[i] = valu;
}


// returns if character is white-space
bool is_white (char str)
{
	return str <= ' ';
}


// returns index of first non white-space character in 'str'
// if all is white-space, returns position of '\0'
int skip_white (LPSTR str, int start)
{
	int i = 0; 
	
	while (is_white (str[i]) && (str[i] != '\0'))
		i++;
	
	return i;
}


// reads a line of text starting at index 'start' ending with '\n'
// lines separated by '\0'
int read_line (LPSTR str, int start, LPSTR result)
{
	int len = strlen (str);

	if (start < len - 1) {
		int n = 0;
		int i;

		for (i = start; (str[i] != '\n') && (i < len); i++)
			result[n++] = str[i];

		result[n] = '\0';
		return i + 1;
	}
	else
		return -1;
}


// removes white space from left end of a string
int no_white_lef (LPSTR str, LPSTR result)
{
	int n = 0, i = 0;
	
	while (is_white (str[i]) && str[i] != '\0')
		i++;

	while (str[i] != '\0') 
		result[n++] = str[i++];

	result[n] = '\0';
	return i - n;
}


// converts number to string in binary
char* cv_string_bin (int n) // doesnt work
{
	static char res[128];
	int p = 0;

	for (int i = 0; i < 32; i++) {
		if (n & (i << (32 - i)))
			res[p++] = '1';
		else
			res[p++] = '0';

		//if ((i & 7) == 7)
		//	res[p++] = ' ';
	}

	res[p++] = '\0';

	return res;
}


#endif // !INC_MY_DATA