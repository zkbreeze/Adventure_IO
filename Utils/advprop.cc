/***********************************************************
Copyright (C) 2000,2001,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include <Adv/AdvUtil.h>
#include <Adv/AdvDocument.h>
#include <cassert>
#include <cstdio>
#include <string>
#include <strstream>
#include <fstream>
#include <list>

void
dump_doc(AdvDocument* in, AdvDocument* out)
{
  char key[2048];
  char val[2048];
  int n;
  n = 0;
  if (adv_dio_get_nth_property(in, n, key, sizeof(key), val, sizeof(val)))
    do
      {
	n++;
	adv_dio_set_property(out, key, val);
      } while (adv_dio_get_nth_property(in, n, key, sizeof(key), val, sizeof(val)));

  adv_off_t left = adv_dio_get_size(in);
  adv_off_t off = 0;
  octet buf[1024*10];
  while (left > 0)
    {
      int32 read = left < sizeof(buf) ? left : sizeof(buf);
      adv_dio_read_octet(in, off, read, buf);
      adv_dio_write_octet(out, off, read, buf);
      off += read;
      left -= read;
    }
}

class OpSet;
class OpRemove;
class OpChangeKey;
class OpChangeVal;

class Operation
{
public:
  static Operation* Create(const char* line);	  // should use instead of ctor
  static void       Destroy(Operation* op)	  // should use instead of dtor
    { delete op; }
  
  virtual void Eval(AdvDocument* doc) = 0;

protected:
  Operation()
    {}
  virtual ~Operation()
    {}
};


// set <key> <val>
class OpSet : public Operation
{
  friend class Operation;
public:
  void Eval(AdvDocument* doc)
    { adv_dio_set_property(doc, key.c_str(), val.c_str()); }
protected:
  OpSet(const char* line)
    {
      istrstream is(line);
      string token;
      is >> token >> key >> val;
      assert(strcasecmp(token.c_str(), "set") == 0);
    }
  ~OpSet()
    {}
private:
  string key;
  string val;
};

// remove <key>
class OpRemove : public Operation
{
  friend class Operation;
public:
  void Eval(AdvDocument* doc)
    {
      int n = 0;
      char k[1024];
      char v[4096];
      if (adv_dio_get_nth_property(doc, n, k, sizeof(k), v, sizeof(v)))
	{
	  do
	    {
	      if (key == k)
		{
		  adv_dio_unset_nth_property(doc, n);
		  break;
		}
	      n++;
	    } while (adv_dio_get_nth_property(doc, n, k, sizeof(k), v, sizeof(v)));
	}
    }
protected:
  OpRemove(const char* line)
    {
      istrstream is(line);
      string token;
      is >> token >> key;
      assert(strcasecmp(token.c_str(), "remove") == 0);
    }
  ~OpRemove()
    {}
private:
  string key;
};

// change_key <oldkey> <newkey>
class OpChangeKey : public Operation
{
  friend class Operation;
public:
  void Eval(AdvDocument* doc)
    {
      int n = 0;
      char k[1024];
      char v[4096];
      if (adv_dio_get_nth_property(doc, n, k, sizeof(k), v, sizeof(v)))
	{
	  do 
	    {
	      if (oldkey == k)
		{
		  adv_dio_unset_nth_property(doc, n);
		  adv_dio_set_property(doc, newkey.c_str(), v);
		  break;
		}
	      n++;
	    } while (adv_dio_get_nth_property(doc, n, k, sizeof(k), v, sizeof(v)));
	}
    }
protected:
  OpChangeKey(const char* line)
    {
      istrstream is(line);
      string token;
      is >> token >> oldkey >> newkey;
      assert(strcasecmp(token.c_str(), "change_key") == 0);
    }
  ~OpChangeKey()
    {}
private:
  string oldkey;
  string newkey;
};

// change_val <key> <oldval> <newval>
class OpChangeVal : public Operation
{
  friend class Operation;
public:
  void Eval(AdvDocument* doc)
    {
      int n = 0;
      char k[1024];
      char v[4096];
      if (adv_dio_get_nth_property(doc, n, k, sizeof(k), v, sizeof(v)))
	{
	  do
	    {
	      if (key == k && oldval == v)
		{
		  adv_dio_set_property(doc, key.c_str(), newval.c_str());
		  break;
		}
	      n++;
	    } while (adv_dio_get_nth_property(doc, n, k, sizeof(k), v, sizeof(v)));
	}
    }
protected:
  OpChangeVal(const char* line)
    {
      istrstream is(line);
      string token;
      is >> token >> key >> oldval >> newval;
      assert(strcasecmp(token.c_str(), "change_val") == 0);
    }
  ~OpChangeVal()
    {}
private:
  string key;
  string oldval;
  string newval;
};

Operation*
Operation::Create(const char* line)
{
  istrstream is(line);
  string token;
  is >> token;
  if (strcasecmp(token.c_str(), "set") == 0)
    return new OpSet(line);
  else if (strcasecmp(token.c_str(), "remove") == 0)
    return new OpRemove(line);
  else if (strcasecmp(token.c_str(), "change_key") == 0)
    return new OpChangeKey(line);
  else if (strcasecmp(token.c_str(), "change_val") == 0)
    return new OpChangeVal(line);
  else
    return NULL;
}

void
adios(int code, const char* msg)
{
  cout << msg << endl;
  exit(code);
}

int
main(int ac, char** av)
{
  if (ac < 4)
    {
      printf("Usage: %s <script> <in>... <outfile>\n", av[0]);
      printf("$Id: advprop.cc,v 1.3.2.1 2006/02/02 10:05:45 tohyama Exp $\n");
      printf("do some operations (set, unset, change, ...) on the properties of specified documents and write out modified documents to a file.\n");
      return 1;
    }

  ifstream ifs(av[1]);
  if (!ifs.good())
    adios(1, "cannot open scriopt file\n");
  string line;
  list<Operation*> oplist;
  while (ifs.good())
    {
      getline(ifs, line);
      Operation* op = Operation::Create(line.c_str());
      if (op)
	oplist.push_back(op);
    }
  
  AdvDatabox* dbox = adv_dbox_new();
  for (int i = 2; i < ac - 1; i++)
    adv_dbox_add(dbox, av[i]);
  
  AdvDocFile* dfile = adv_dio_file_open(av[ac-1], "c");
  AdvDocument* in;
  AdvDocument* out;
  int n = 0;
  if (in = adv_dbox_open_nth(dbox, n))
    do
      {
	n++;
	out = adv_dio_create(dfile, adv_dio_make_documentid("did"));
	dump_doc(in, out);
	for (list<Operation*>::iterator it = oplist.begin(); it != oplist.end(); it++)
	  (*it)->Eval(out);
	adv_dio_close(out);
      } while (in = adv_dbox_open_nth(dbox, n));
  adv_dio_file_close(dfile);
}
