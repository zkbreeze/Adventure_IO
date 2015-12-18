/*
 *  MICO --- a free CORBA implementation
 *  Copyright (C) 1997-98 Kay Roemer & Arno Puder
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#if !defined(__ADV_H__) || defined(MICO_NO_TOPLEVEL_MODULES)
#define __ADV_H__

#ifndef MICO_NO_TOPLEVEL_MODULES
#include <CORBA.h>
#include <mico/throw.h>
#endif

#ifndef MICO_NO_TOPLEVEL_MODULES
MICO_NAMESPACE_DECL Adv {
#endif

#if !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE_Adv)


typedef CORBA::Char Int8;
typedef CORBA::Short Int16;
typedef CORBA::Long Int32;
typedef CORBA::LongLong Int64;
typedef CORBA::Float Float32;
typedef CORBA::Double Float64;
typedef IfaceSequenceTmpl<CORBA::Object_var,CORBA::Object_ptr> ObjectSeq;
#ifdef _WINDOWS
static ObjectSeq _dummy_ObjectSeq;
#endif
typedef TSeqVar<IfaceSequenceTmpl<CORBA::Object_var,CORBA::Object_ptr> > ObjectSeq_var;
typedef ObjectSeq_var ObjectSeq_out;

typedef SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> OctetSeq;
#ifdef _WINDOWS
static OctetSeq _dummy_OctetSeq;
#endif
typedef TSeqVar<SequenceTmpl<CORBA::Octet,MICO_TID_OCTET> > OctetSeq_var;
typedef OctetSeq_var OctetSeq_out;

typedef StringSequenceTmpl<CORBA::String_var> StringSeq;
#ifdef _WINDOWS
static StringSeq _dummy_StringSeq;
#endif
typedef TSeqVar<StringSequenceTmpl<CORBA::String_var> > StringSeq_var;
typedef StringSeq_var StringSeq_out;

typedef SequenceTmpl<CORBA::Char,MICO_TID_CHAR> Int8Seq;
#ifdef _WINDOWS
static Int8Seq _dummy_Int8Seq;
#endif
typedef TSeqVar<SequenceTmpl<CORBA::Char,MICO_TID_CHAR> > Int8Seq_var;
typedef Int8Seq_var Int8Seq_out;

typedef SequenceTmpl<CORBA::Short,MICO_TID_DEF> Int16Seq;
#ifdef _WINDOWS
static Int16Seq _dummy_Int16Seq;
#endif
typedef TSeqVar<SequenceTmpl<CORBA::Short,MICO_TID_DEF> > Int16Seq_var;
typedef Int16Seq_var Int16Seq_out;

typedef SequenceTmpl<CORBA::Long,MICO_TID_DEF> Int32Seq;
#ifdef _WINDOWS
static Int32Seq _dummy_Int32Seq;
#endif
typedef TSeqVar<SequenceTmpl<CORBA::Long,MICO_TID_DEF> > Int32Seq_var;
typedef Int32Seq_var Int32Seq_out;

typedef SequenceTmpl<CORBA::Float,MICO_TID_DEF> Float32Seq;
#ifdef _WINDOWS
static Float32Seq _dummy_Float32Seq;
#endif
typedef TSeqVar<SequenceTmpl<CORBA::Float,MICO_TID_DEF> > Float32Seq_var;
typedef Float32Seq_var Float32Seq_out;

typedef SequenceTmpl<CORBA::Double,MICO_TID_DEF> Float64Seq;
#ifdef _WINDOWS
static Float64Seq _dummy_Float64Seq;
#endif
typedef TSeqVar<SequenceTmpl<CORBA::Double,MICO_TID_DEF> > Float64Seq_var;
typedef Float64Seq_var Float64Seq_out;

class HostManager;
typedef HostManager *HostManager_ptr;
typedef HostManager_ptr HostManagerRef;
typedef ObjVar<HostManager> HostManager_var;
typedef HostManager_var HostManager_out;

typedef IfaceSequenceTmpl<HostManager_var,HostManager_ptr> HostManagerSeq;
#ifdef _WINDOWS
static HostManagerSeq _dummy_HostManagerSeq;
#endif
typedef TSeqVar<IfaceSequenceTmpl<HostManager_var,HostManager_ptr> > HostManagerSeq_var;
typedef HostManagerSeq_var HostManagerSeq_out;

MICO_EXPORT_CONST_DECL const char* HostManagerIORFile;
MICO_EXPORT_CONST_DECL const char* SessionManagerIORFile;
struct Event;
typedef TVarVar<Event> Event_var;
typedef Event_var Event_out;

struct Event {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef Event_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  Event();
  ~Event();
  Event( const Event& s );
  Event& operator=( const Event& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::Object_var who;
  CORBA::String_var what;
  CORBA::Any param;
};

struct HostState;
typedef TFixVar<HostState> HostState_var;
typedef HostState_var HostState_out;

struct HostState {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef HostState_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  HostState();
  ~HostState();
  HostState( const HostState& s );
  HostState& operator=( const HostState& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::LongLong availablememory;
  CORBA::Float load[ 3 ];
};

struct NamedValue;
typedef TVarVar<NamedValue> NamedValue_var;
typedef NamedValue_var NamedValue_out;

struct NamedValue {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef NamedValue_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  NamedValue();
  ~NamedValue();
  NamedValue( const NamedValue& s );
  NamedValue& operator=( const NamedValue& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::String_var name;
  CORBA::String_var value;
};

typedef SequenceTmpl<NamedValue,MICO_TID_DEF> NamedValueSeq;
#ifdef _WINDOWS
static NamedValueSeq _dummy_NamedValueSeq;
#endif
typedef TSeqVar<SequenceTmpl<NamedValue,MICO_TID_DEF> > NamedValueSeq_var;
typedef NamedValueSeq_var NamedValueSeq_out;

class Component;
typedef Component *Component_ptr;
typedef Component_ptr ComponentRef;
typedef ObjVar<Component> Component_var;
typedef Component_var Component_out;

class SessionManager;
typedef SessionManager *SessionManager_ptr;
typedef SessionManager_ptr SessionManagerRef;
typedef ObjVar<SessionManager> SessionManager_var;
typedef SessionManager_var SessionManager_out;


// Common definitions for interface SessionManager
class SessionManager : 
  virtual public CORBA::Object
{
  public:
    virtual ~SessionManager();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef SessionManager_ptr _ptr_type;
    typedef SessionManager_var _var_type;
    #endif

    static SessionManager_ptr _narrow( CORBA::Object_ptr obj );
    static SessionManager_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static SessionManager_ptr _duplicate( SessionManager_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static SessionManager_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );
    static vector<CORBA::Narrow_proto> *_narrow_helpers;
    static bool _narrow_helper2( CORBA::Object_ptr obj );

    struct NotFound : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      NotFound();
      ~NotFound();
      NotFound( const NotFound& s );
      NotFound& operator=( const NotFound& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      NotFound *operator->() { return this; }
      NotFound& operator*() { return *this; }
      operator NotFound*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static NotFound *EXCEPTION_DOWNCAST( CORBA::Exception *ex );
      static const NotFound *EXCEPTION_DOWNCAST( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef NotFound NotFound_catch;
    #else
    typedef ExceptVar<NotFound> NotFound_var;
    typedef NotFound_var NotFound_out;
    typedef NotFound_var NotFound_catch;
    #endif // HAVE_STD_EH

    virtual char* GetUserName() = 0;
    virtual HostManagerSeq* GetHostManagers() = 0;
    virtual char* GetLocation() = 0;
    virtual void ResistComponent( Component_ptr component ) = 0;
    virtual StringSeq* GetHTML( const char* method, const char* object, const NamedValueSeq& header, const NamedValueSeq& params ) = 0;

  protected:
    SessionManager() {};
  private:
    SessionManager( const SessionManager& );
    void operator=( const SessionManager& );
};

// Stub for interface SessionManager
class SessionManager_stub:
  virtual public SessionManager
{
  public:
    virtual ~SessionManager_stub();
    char* GetUserName();
    HostManagerSeq* GetHostManagers();
    char* GetLocation();
    void ResistComponent( Component_ptr component );
    StringSeq* GetHTML( const char* method, const char* object, const NamedValueSeq& header, const NamedValueSeq& params );

  private:
    void operator=( const SessionManager_stub& );
};

#ifndef MICO_CONF_NO_POA

class SessionManager_stub_clp :
  virtual public SessionManager_stub,
  virtual public PortableServer::StubBase
{
  public:
    SessionManager_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~SessionManager_stub_clp ();
    char* GetUserName();
    HostManagerSeq* GetHostManagers();
    char* GetLocation();
    void ResistComponent( Component_ptr component );
    StringSeq* GetHTML( const char* method, const char* object, const NamedValueSeq& header, const NamedValueSeq& params );

  protected:
    SessionManager_stub_clp ();
  private:
    void operator=( const SessionManager_stub_clp & );
};

#endif // MICO_CONF_NO_POA

class ClusterManager;
typedef ClusterManager *ClusterManager_ptr;
typedef ClusterManager_ptr ClusterManagerRef;
typedef ObjVar<ClusterManager> ClusterManager_var;
typedef ClusterManager_var ClusterManager_out;


// Common definitions for interface ClusterManager
class ClusterManager : 
  virtual public CORBA::Object
{
  public:
    virtual ~ClusterManager();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ClusterManager_ptr _ptr_type;
    typedef ClusterManager_var _var_type;
    #endif

    static ClusterManager_ptr _narrow( CORBA::Object_ptr obj );
    static ClusterManager_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ClusterManager_ptr _duplicate( ClusterManager_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ClusterManager_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );
    static vector<CORBA::Narrow_proto> *_narrow_helpers;
    static bool _narrow_helper2( CORBA::Object_ptr obj );

    virtual void ResistHostManager( HostManager_ptr hostmanager ) = 0;
    virtual HostManagerSeq* GetHostManagers() = 0;

  protected:
    ClusterManager() {};
  private:
    ClusterManager( const ClusterManager& );
    void operator=( const ClusterManager& );
};

// Stub for interface ClusterManager
class ClusterManager_stub:
  virtual public ClusterManager
{
  public:
    virtual ~ClusterManager_stub();
    void ResistHostManager( HostManager_ptr hostmanager );
    HostManagerSeq* GetHostManagers();

  private:
    void operator=( const ClusterManager_stub& );
};

#ifndef MICO_CONF_NO_POA

class ClusterManager_stub_clp :
  virtual public ClusterManager_stub,
  virtual public PortableServer::StubBase
{
  public:
    ClusterManager_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ClusterManager_stub_clp ();
    void ResistHostManager( HostManager_ptr hostmanager );
    HostManagerSeq* GetHostManagers();

  protected:
    ClusterManager_stub_clp ();
  private:
    void operator=( const ClusterManager_stub_clp & );
};

#endif // MICO_CONF_NO_POA

class ProcessFactory;
typedef ProcessFactory *ProcessFactory_ptr;
typedef ProcessFactory_ptr ProcessFactoryRef;
typedef ObjVar<ProcessFactory> ProcessFactory_var;
typedef ProcessFactory_var ProcessFactory_out;

class DocumentManager;
typedef DocumentManager *DocumentManager_ptr;
typedef DocumentManager_ptr DocumentManagerRef;
typedef ObjVar<DocumentManager> DocumentManager_var;
typedef DocumentManager_var DocumentManager_out;


// Common definitions for interface HostManager
class HostManager : 
  virtual public CORBA::Object
{
  public:
    virtual ~HostManager();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef HostManager_ptr _ptr_type;
    typedef HostManager_var _var_type;
    #endif

    static HostManager_ptr _narrow( CORBA::Object_ptr obj );
    static HostManager_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static HostManager_ptr _duplicate( HostManager_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static HostManager_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );
    static vector<CORBA::Narrow_proto> *_narrow_helpers;
    static bool _narrow_helper2( CORBA::Object_ptr obj );

    virtual char* GetHostName() = 0;
    virtual char* GetHostIP() = 0;
    virtual HostState GetHostState() = 0;
    virtual ProcessFactory_ptr GetProcessFactory() = 0;
    virtual DocumentManager_ptr GetDocumentManager() = 0;

  protected:
    HostManager() {};
  private:
    HostManager( const HostManager& );
    void operator=( const HostManager& );
};

// Stub for interface HostManager
class HostManager_stub:
  virtual public HostManager
{
  public:
    virtual ~HostManager_stub();
    char* GetHostName();
    char* GetHostIP();
    HostState GetHostState();
    ProcessFactory_ptr GetProcessFactory();
    DocumentManager_ptr GetDocumentManager();

  private:
    void operator=( const HostManager_stub& );
};

#ifndef MICO_CONF_NO_POA

class HostManager_stub_clp :
  virtual public HostManager_stub,
  virtual public PortableServer::StubBase
{
  public:
    HostManager_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~HostManager_stub_clp ();
    char* GetHostName();
    char* GetHostIP();
    HostState GetHostState();
    ProcessFactory_ptr GetProcessFactory();
    DocumentManager_ptr GetDocumentManager();

  protected:
    HostManager_stub_clp ();
  private:
    void operator=( const HostManager_stub_clp & );
};

#endif // MICO_CONF_NO_POA


// Common definitions for interface DocumentManager
class DocumentManager : 
  virtual public CORBA::Object
{
  public:
    virtual ~DocumentManager();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DocumentManager_ptr _ptr_type;
    typedef DocumentManager_var _var_type;
    #endif

    static DocumentManager_ptr _narrow( CORBA::Object_ptr obj );
    static DocumentManager_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DocumentManager_ptr _duplicate( DocumentManager_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DocumentManager_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );
    static vector<CORBA::Narrow_proto> *_narrow_helpers;
    static bool _narrow_helper2( CORBA::Object_ptr obj );

    struct CannotFind : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      CannotFind();
      ~CannotFind();
      CannotFind( const CannotFind& s );
      CannotFind& operator=( const CannotFind& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      CannotFind *operator->() { return this; }
      CannotFind& operator*() { return *this; }
      operator CannotFind*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static CannotFind *EXCEPTION_DOWNCAST( CORBA::Exception *ex );
      static const CannotFind *EXCEPTION_DOWNCAST( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef CannotFind CannotFind_catch;
    #else
    typedef ExceptVar<CannotFind> CannotFind_var;
    typedef CannotFind_var CannotFind_out;
    typedef CannotFind_var CannotFind_catch;
    #endif // HAVE_STD_EH

    virtual char* Export( const char* locator ) = 0;
    virtual char* Unexport( const char* locator ) = 0;
    virtual StringSeq* GetList() = 0;
    virtual char* QueryDocumentID( const char* id ) = 0;
    virtual char* QueryLocator( const char* locator ) = 0;

  protected:
    DocumentManager() {};
  private:
    DocumentManager( const DocumentManager& );
    void operator=( const DocumentManager& );
};

// Stub for interface DocumentManager
class DocumentManager_stub:
  virtual public DocumentManager
{
  public:
    virtual ~DocumentManager_stub();
    char* Export( const char* locator );
    char* Unexport( const char* locator );
    StringSeq* GetList();
    char* QueryDocumentID( const char* id );
    char* QueryLocator( const char* locator );

  private:
    void operator=( const DocumentManager_stub& );
};

#ifndef MICO_CONF_NO_POA

class DocumentManager_stub_clp :
  virtual public DocumentManager_stub,
  virtual public PortableServer::StubBase
{
  public:
    DocumentManager_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~DocumentManager_stub_clp ();
    char* Export( const char* locator );
    char* Unexport( const char* locator );
    StringSeq* GetList();
    char* QueryDocumentID( const char* id );
    char* QueryLocator( const char* locator );

  protected:
    DocumentManager_stub_clp ();
  private:
    void operator=( const DocumentManager_stub_clp & );
};

#endif // MICO_CONF_NO_POA


// Common definitions for interface ProcessFactory
class ProcessFactory : 
  virtual public CORBA::Object
{
  public:
    virtual ~ProcessFactory();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef ProcessFactory_ptr _ptr_type;
    typedef ProcessFactory_var _var_type;
    #endif

    static ProcessFactory_ptr _narrow( CORBA::Object_ptr obj );
    static ProcessFactory_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static ProcessFactory_ptr _duplicate( ProcessFactory_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static ProcessFactory_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );
    static vector<CORBA::Narrow_proto> *_narrow_helpers;
    static bool _narrow_helper2( CORBA::Object_ptr obj );

    virtual void Execute( const char* modulepath ) = 0;

  protected:
    ProcessFactory() {};
  private:
    ProcessFactory( const ProcessFactory& );
    void operator=( const ProcessFactory& );
};

// Stub for interface ProcessFactory
class ProcessFactory_stub:
  virtual public ProcessFactory
{
  public:
    virtual ~ProcessFactory_stub();
    void Execute( const char* modulepath );

  private:
    void operator=( const ProcessFactory_stub& );
};

#ifndef MICO_CONF_NO_POA

class ProcessFactory_stub_clp :
  virtual public ProcessFactory_stub,
  virtual public PortableServer::StubBase
{
  public:
    ProcessFactory_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~ProcessFactory_stub_clp ();
    void Execute( const char* modulepath );

  protected:
    ProcessFactory_stub_clp ();
  private:
    void operator=( const ProcessFactory_stub_clp & );
};

#endif // MICO_CONF_NO_POA


// Common definitions for interface Component
class Component : 
  virtual public CORBA::Object
{
  public:
    virtual ~Component();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Component_ptr _ptr_type;
    typedef Component_var _var_type;
    #endif

    static Component_ptr _narrow( CORBA::Object_ptr obj );
    static Component_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Component_ptr _duplicate( Component_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Component_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );
    static vector<CORBA::Narrow_proto> *_narrow_helpers;
    static bool _narrow_helper2( CORBA::Object_ptr obj );

    virtual char* GetName() = 0;
    virtual void Deactivate() = 0;
    virtual void ConnectObserver( Component_ptr obs ) = 0;
    virtual void Pushed( const Event& event ) = 0;
    virtual StringSeq* GetHTML( const char* method, const NamedValueSeq& header, const NamedValueSeq& params ) = 0;

  protected:
    Component() {};
  private:
    Component( const Component& );
    void operator=( const Component& );
};

// Stub for interface Component
class Component_stub:
  virtual public Component
{
  public:
    virtual ~Component_stub();
    char* GetName();
    void Deactivate();
    void ConnectObserver( Component_ptr obs );
    void Pushed( const Event& event );
    StringSeq* GetHTML( const char* method, const NamedValueSeq& header, const NamedValueSeq& params );

  private:
    void operator=( const Component_stub& );
};

#ifndef MICO_CONF_NO_POA

class Component_stub_clp :
  virtual public Component_stub,
  virtual public PortableServer::StubBase
{
  public:
    Component_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~Component_stub_clp ();
    char* GetName();
    void Deactivate();
    void ConnectObserver( Component_ptr obs );
    void Pushed( const Event& event );
    StringSeq* GetHTML( const char* method, const NamedValueSeq& header, const NamedValueSeq& params );

  protected:
    Component_stub_clp ();
  private:
    void operator=( const Component_stub_clp & );
};

#endif // MICO_CONF_NO_POA

class DocAccessor;
typedef DocAccessor *DocAccessor_ptr;
typedef DocAccessor_ptr DocAccessorRef;
typedef ObjVar<DocAccessor> DocAccessor_var;
typedef DocAccessor_var DocAccessor_out;


// Common definitions for interface DocAccessor
class DocAccessor : 
  virtual public CORBA::Object
{
  public:
    virtual ~DocAccessor();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DocAccessor_ptr _ptr_type;
    typedef DocAccessor_var _var_type;
    #endif

    static DocAccessor_ptr _narrow( CORBA::Object_ptr obj );
    static DocAccessor_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DocAccessor_ptr _duplicate( DocAccessor_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DocAccessor_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );
    static vector<CORBA::Narrow_proto> *_narrow_helpers;
    static bool _narrow_helper2( CORBA::Object_ptr obj );

    virtual char* GetDocumentID() = 0;
    virtual CORBA::Boolean GetNthProperty( CORBA::Long n, char*& key, char*& val ) = 0;
    virtual CORBA::Boolean GetProperty( const char* key, char*& val ) = 0;
    virtual void SetProperty( const char* key, const char* val ) = 0;
    virtual void UnsetNthProperty( CORBA::Long n ) = 0;
    virtual CORBA::Long GetSize() = 0;
    virtual CORBA::Long GetOctet( Int64 offset, Int32 length, OctetSeq*& buffer ) = 0;
    virtual CORBA::Long SetOctet( Int64 offset, const OctetSeq& buffer ) = 0;

  protected:
    DocAccessor() {};
  private:
    DocAccessor( const DocAccessor& );
    void operator=( const DocAccessor& );
};

// Stub for interface DocAccessor
class DocAccessor_stub:
  virtual public DocAccessor
{
  public:
    virtual ~DocAccessor_stub();
    char* GetDocumentID();
    CORBA::Boolean GetNthProperty( CORBA::Long n, char*& key, char*& val );
    CORBA::Boolean GetProperty( const char* key, char*& val );
    void SetProperty( const char* key, const char* val );
    void UnsetNthProperty( CORBA::Long n );
    CORBA::Long GetSize();
    CORBA::Long GetOctet( Int64 offset, Int32 length, OctetSeq*& buffer );
    CORBA::Long SetOctet( Int64 offset, const OctetSeq& buffer );

  private:
    void operator=( const DocAccessor_stub& );
};

#ifndef MICO_CONF_NO_POA

class DocAccessor_stub_clp :
  virtual public DocAccessor_stub,
  virtual public PortableServer::StubBase
{
  public:
    DocAccessor_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~DocAccessor_stub_clp ();
    char* GetDocumentID();
    CORBA::Boolean GetNthProperty( CORBA::Long n, char*& key, char*& val );
    CORBA::Boolean GetProperty( const char* key, char*& val );
    void SetProperty( const char* key, const char* val );
    void UnsetNthProperty( CORBA::Long n );
    CORBA::Long GetSize();
    CORBA::Long GetOctet( Int64 offset, Int32 length, OctetSeq*& buffer );
    CORBA::Long SetOctet( Int64 offset, const OctetSeq& buffer );

  protected:
    DocAccessor_stub_clp ();
  private:
    void operator=( const DocAccessor_stub_clp & );
};

#endif // MICO_CONF_NO_POA

class DocStorage;
typedef DocStorage *DocStorage_ptr;
typedef DocStorage_ptr DocStorageRef;
typedef ObjVar<DocStorage> DocStorage_var;
typedef DocStorage_var DocStorage_out;


// Common definitions for interface DocStorage
class DocStorage : 
  virtual public CORBA::Object
{
  public:
    virtual ~DocStorage();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef DocStorage_ptr _ptr_type;
    typedef DocStorage_var _var_type;
    #endif

    static DocStorage_ptr _narrow( CORBA::Object_ptr obj );
    static DocStorage_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static DocStorage_ptr _duplicate( DocStorage_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static DocStorage_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );
    static vector<CORBA::Narrow_proto> *_narrow_helpers;
    static bool _narrow_helper2( CORBA::Object_ptr obj );

    struct NoSuchDocument : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      NoSuchDocument();
      ~NoSuchDocument();
      NoSuchDocument( const NoSuchDocument& s );
      NoSuchDocument& operator=( const NoSuchDocument& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      NoSuchDocument *operator->() { return this; }
      NoSuchDocument& operator*() { return *this; }
      operator NoSuchDocument*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static NoSuchDocument *EXCEPTION_DOWNCAST( CORBA::Exception *ex );
      static const NoSuchDocument *EXCEPTION_DOWNCAST( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef NoSuchDocument NoSuchDocument_catch;
    #else
    typedef ExceptVar<NoSuchDocument> NoSuchDocument_var;
    typedef NoSuchDocument_var NoSuchDocument_out;
    typedef NoSuchDocument_var NoSuchDocument_catch;
    #endif // HAVE_STD_EH

    struct NotEnoughMemory : public CORBA::UserException {
      #ifdef HAVE_EXPLICIT_STRUCT_OPS
      NotEnoughMemory();
      ~NotEnoughMemory();
      NotEnoughMemory( const NotEnoughMemory& s );
      NotEnoughMemory& operator=( const NotEnoughMemory& s );
      #endif //HAVE_EXPLICIT_STRUCT_OPS


      #ifdef HAVE_STD_EH
      NotEnoughMemory *operator->() { return this; }
      NotEnoughMemory& operator*() { return *this; }
      operator NotEnoughMemory*() { return this; }
      #endif // HAVE_STD_EH

      void _throwit() const;
      const char *_repoid() const;
      void _encode( CORBA::DataEncoder &en ) const;
      void _encode_any( CORBA::Any &a ) const;
      CORBA::Exception *_clone() const;
      static NotEnoughMemory *EXCEPTION_DOWNCAST( CORBA::Exception *ex );
      static const NotEnoughMemory *EXCEPTION_DOWNCAST( const CORBA::Exception *ex );
    };

    #ifdef HAVE_STD_EH
    typedef NotEnoughMemory NotEnoughMemory_catch;
    #else
    typedef ExceptVar<NotEnoughMemory> NotEnoughMemory_var;
    typedef NotEnoughMemory_var NotEnoughMemory_out;
    typedef NotEnoughMemory_var NotEnoughMemory_catch;
    #endif // HAVE_STD_EH

    virtual CORBA::Long GetNumDocuments() = 0;
    virtual void Clear() = 0;
    virtual DocAccessor_ptr Create( const char* documentid ) = 0;
    virtual DocAccessor_ptr OpenNth( CORBA::Long n ) = 0;
    virtual DocAccessor_ptr OpenByDocumentID( const char* documentid ) = 0;
    virtual DocAccessor_ptr OpenByProperty( DocAccessor_ptr prev, const StringSeq& properties ) = 0;

  protected:
    DocStorage() {};
  private:
    DocStorage( const DocStorage& );
    void operator=( const DocStorage& );
};

// Stub for interface DocStorage
class DocStorage_stub:
  virtual public DocStorage
{
  public:
    virtual ~DocStorage_stub();
    CORBA::Long GetNumDocuments();
    void Clear();
    DocAccessor_ptr Create( const char* documentid );
    DocAccessor_ptr OpenNth( CORBA::Long n );
    DocAccessor_ptr OpenByDocumentID( const char* documentid );
    DocAccessor_ptr OpenByProperty( DocAccessor_ptr prev, const StringSeq& properties );

  private:
    void operator=( const DocStorage_stub& );
};

#ifndef MICO_CONF_NO_POA

class DocStorage_stub_clp :
  virtual public DocStorage_stub,
  virtual public PortableServer::StubBase
{
  public:
    DocStorage_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~DocStorage_stub_clp ();
    CORBA::Long GetNumDocuments();
    void Clear();
    DocAccessor_ptr Create( const char* documentid );
    DocAccessor_ptr OpenNth( CORBA::Long n );
    DocAccessor_ptr OpenByDocumentID( const char* documentid );
    DocAccessor_ptr OpenByProperty( DocAccessor_ptr prev, const StringSeq& properties );

  protected:
    DocStorage_stub_clp ();
  private:
    void operator=( const DocStorage_stub_clp & );
};

#endif // MICO_CONF_NO_POA

class Container;
typedef Container *Container_ptr;
typedef Container_ptr ContainerRef;
typedef ObjVar<Container> Container_var;
typedef Container_var Container_out;


// Common definitions for interface Container
class Container : 
  virtual public ::Adv::Component
{
  public:
    virtual ~Container();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef Container_ptr _ptr_type;
    typedef Container_var _var_type;
    #endif

    static Container_ptr _narrow( CORBA::Object_ptr obj );
    static Container_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static Container_ptr _duplicate( Container_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static Container_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );
    static vector<CORBA::Narrow_proto> *_narrow_helpers;
    static bool _narrow_helper2( CORBA::Object_ptr obj );

  protected:
    Container() {};
  private:
    Container( const Container& );
    void operator=( const Container& );
};

// Stub for interface Container
class Container_stub:
  virtual public Container,
  virtual public ::Adv::Component_stub
{
  public:
    virtual ~Container_stub();
  private:
    void operator=( const Container_stub& );
};

#ifndef MICO_CONF_NO_POA

class Container_stub_clp :
  virtual public Container_stub,
  virtual public ::Adv::Component_stub_clp
{
  public:
    Container_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~Container_stub_clp ();
  protected:
    Container_stub_clp ();
  private:
    void operator=( const Container_stub_clp & );
};

#endif // MICO_CONF_NO_POA

#endif // !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE_Adv)

#ifndef MICO_NO_TOPLEVEL_MODULES

};
#endif



#ifndef MICO_CONF_NO_POA

#ifndef MICO_NO_TOPLEVEL_MODULES
MICO_NAMESPACE_DECL POA_Adv {
#endif

#if !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE_POA_Adv)


class SessionManager : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~SessionManager ();
    Adv::SessionManager_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static SessionManager * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual char* GetUserName() = 0;
    virtual Adv::HostManagerSeq* GetHostManagers() = 0;
    virtual char* GetLocation() = 0;
    virtual void ResistComponent( Adv::Component_ptr component ) = 0;
    virtual Adv::StringSeq* GetHTML( const char* method, const char* object, const Adv::NamedValueSeq& header, const Adv::NamedValueSeq& params ) = 0;

  protected:
    SessionManager () {};

  private:
    SessionManager (const SessionManager &);
    void operator= (const SessionManager &);
};

#ifdef HAVE_NAMESPACE

template<class T>
class SessionManager_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public SessionManager
{
  public:
    SessionManager_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    SessionManager_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    SessionManager_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    SessionManager_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~SessionManager_tie ();

    PortableServer::POA_ptr _default_POA ();

    char* GetUserName();
    Adv::HostManagerSeq* GetHostManagers();
    char* GetLocation();
    void ResistComponent( Adv::Component_ptr component );
    Adv::StringSeq* GetHTML( const char* method, const char* object, const Adv::NamedValueSeq& header, const Adv::NamedValueSeq& params );

  protected:
    SessionManager_tie () {};

  private:
    SessionManager_tie (const SessionManager_tie<T> &);
    void operator= (const SessionManager_tie<T> &);
};

template<class T>
SessionManager_tie<T>::~SessionManager_tie ()
{
}

template<class T>
PortableServer::POA_ptr
SessionManager_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
char*
SessionManager_tie<T>::GetUserName ()
{
  return POA_Tie_Base<T>::ptr->GetUserName ();
}

template<class T>
Adv::HostManagerSeq*
SessionManager_tie<T>::GetHostManagers ()
{
  return POA_Tie_Base<T>::ptr->GetHostManagers ();
}

template<class T>
char*
SessionManager_tie<T>::GetLocation ()
{
  return POA_Tie_Base<T>::ptr->GetLocation ();
}

template<class T>
void
SessionManager_tie<T>::ResistComponent (Adv::Component_ptr component)
{
  POA_Tie_Base<T>::ptr->ResistComponent (component);
}

template<class T>
Adv::StringSeq*
SessionManager_tie<T>::GetHTML (const char* method, const char* object, const Adv::NamedValueSeq& header, const Adv::NamedValueSeq& params)
{
  return POA_Tie_Base<T>::ptr->GetHTML (method, object, header, params);
}

#endif

class ClusterManager : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~ClusterManager ();
    Adv::ClusterManager_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ClusterManager * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual void ResistHostManager( Adv::HostManager_ptr hostmanager ) = 0;
    virtual Adv::HostManagerSeq* GetHostManagers() = 0;

  protected:
    ClusterManager () {};

  private:
    ClusterManager (const ClusterManager &);
    void operator= (const ClusterManager &);
};

#ifdef HAVE_NAMESPACE

template<class T>
class ClusterManager_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public ClusterManager
{
  public:
    ClusterManager_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    ClusterManager_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    ClusterManager_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    ClusterManager_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~ClusterManager_tie ();

    PortableServer::POA_ptr _default_POA ();

    void ResistHostManager( Adv::HostManager_ptr hostmanager );
    Adv::HostManagerSeq* GetHostManagers();

  protected:
    ClusterManager_tie () {};

  private:
    ClusterManager_tie (const ClusterManager_tie<T> &);
    void operator= (const ClusterManager_tie<T> &);
};

template<class T>
ClusterManager_tie<T>::~ClusterManager_tie ()
{
}

template<class T>
PortableServer::POA_ptr
ClusterManager_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
void
ClusterManager_tie<T>::ResistHostManager (Adv::HostManager_ptr hostmanager)
{
  POA_Tie_Base<T>::ptr->ResistHostManager (hostmanager);
}

template<class T>
Adv::HostManagerSeq*
ClusterManager_tie<T>::GetHostManagers ()
{
  return POA_Tie_Base<T>::ptr->GetHostManagers ();
}

#endif

class HostManager : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~HostManager ();
    Adv::HostManager_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static HostManager * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual char* GetHostName() = 0;
    virtual char* GetHostIP() = 0;
    virtual Adv::HostState GetHostState() = 0;
    virtual Adv::ProcessFactory_ptr GetProcessFactory() = 0;
    virtual Adv::DocumentManager_ptr GetDocumentManager() = 0;

  protected:
    HostManager () {};

  private:
    HostManager (const HostManager &);
    void operator= (const HostManager &);
};

#ifdef HAVE_NAMESPACE

template<class T>
class HostManager_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public HostManager
{
  public:
    HostManager_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    HostManager_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    HostManager_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    HostManager_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~HostManager_tie ();

    PortableServer::POA_ptr _default_POA ();

    char* GetHostName();
    char* GetHostIP();
    Adv::HostState GetHostState();
    Adv::ProcessFactory_ptr GetProcessFactory();
    Adv::DocumentManager_ptr GetDocumentManager();

  protected:
    HostManager_tie () {};

  private:
    HostManager_tie (const HostManager_tie<T> &);
    void operator= (const HostManager_tie<T> &);
};

template<class T>
HostManager_tie<T>::~HostManager_tie ()
{
}

template<class T>
PortableServer::POA_ptr
HostManager_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
char*
HostManager_tie<T>::GetHostName ()
{
  return POA_Tie_Base<T>::ptr->GetHostName ();
}

template<class T>
char*
HostManager_tie<T>::GetHostIP ()
{
  return POA_Tie_Base<T>::ptr->GetHostIP ();
}

template<class T>
Adv::HostState
HostManager_tie<T>::GetHostState ()
{
  return POA_Tie_Base<T>::ptr->GetHostState ();
}

template<class T>
Adv::ProcessFactory_ptr
HostManager_tie<T>::GetProcessFactory ()
{
  return POA_Tie_Base<T>::ptr->GetProcessFactory ();
}

template<class T>
Adv::DocumentManager_ptr
HostManager_tie<T>::GetDocumentManager ()
{
  return POA_Tie_Base<T>::ptr->GetDocumentManager ();
}

#endif

class DocumentManager : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~DocumentManager ();
    Adv::DocumentManager_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static DocumentManager * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual char* Export( const char* locator ) = 0;
    virtual char* Unexport( const char* locator ) = 0;
    virtual Adv::StringSeq* GetList() = 0;
    virtual char* QueryDocumentID( const char* id ) = 0;
    virtual char* QueryLocator( const char* locator ) = 0;

  protected:
    DocumentManager () {};

  private:
    DocumentManager (const DocumentManager &);
    void operator= (const DocumentManager &);
};

#ifdef HAVE_NAMESPACE

template<class T>
class DocumentManager_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public DocumentManager
{
  public:
    DocumentManager_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    DocumentManager_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    DocumentManager_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    DocumentManager_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~DocumentManager_tie ();

    PortableServer::POA_ptr _default_POA ();

    char* Export( const char* locator );
    char* Unexport( const char* locator );
    Adv::StringSeq* GetList();
    char* QueryDocumentID( const char* id );
    char* QueryLocator( const char* locator );

  protected:
    DocumentManager_tie () {};

  private:
    DocumentManager_tie (const DocumentManager_tie<T> &);
    void operator= (const DocumentManager_tie<T> &);
};

template<class T>
DocumentManager_tie<T>::~DocumentManager_tie ()
{
}

template<class T>
PortableServer::POA_ptr
DocumentManager_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
char*
DocumentManager_tie<T>::Export (const char* locator)
{
  return POA_Tie_Base<T>::ptr->Export (locator);
}

template<class T>
char*
DocumentManager_tie<T>::Unexport (const char* locator)
{
  return POA_Tie_Base<T>::ptr->Unexport (locator);
}

template<class T>
Adv::StringSeq*
DocumentManager_tie<T>::GetList ()
{
  return POA_Tie_Base<T>::ptr->GetList ();
}

template<class T>
char*
DocumentManager_tie<T>::QueryDocumentID (const char* id)
{
  return POA_Tie_Base<T>::ptr->QueryDocumentID (id);
}

template<class T>
char*
DocumentManager_tie<T>::QueryLocator (const char* locator)
{
  return POA_Tie_Base<T>::ptr->QueryLocator (locator);
}

#endif

class ProcessFactory : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~ProcessFactory ();
    Adv::ProcessFactory_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static ProcessFactory * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual void Execute( const char* modulepath ) = 0;

  protected:
    ProcessFactory () {};

  private:
    ProcessFactory (const ProcessFactory &);
    void operator= (const ProcessFactory &);
};

#ifdef HAVE_NAMESPACE

template<class T>
class ProcessFactory_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public ProcessFactory
{
  public:
    ProcessFactory_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    ProcessFactory_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    ProcessFactory_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    ProcessFactory_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~ProcessFactory_tie ();

    PortableServer::POA_ptr _default_POA ();

    void Execute( const char* modulepath );

  protected:
    ProcessFactory_tie () {};

  private:
    ProcessFactory_tie (const ProcessFactory_tie<T> &);
    void operator= (const ProcessFactory_tie<T> &);
};

template<class T>
ProcessFactory_tie<T>::~ProcessFactory_tie ()
{
}

template<class T>
PortableServer::POA_ptr
ProcessFactory_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
void
ProcessFactory_tie<T>::Execute (const char* modulepath)
{
  POA_Tie_Base<T>::ptr->Execute (modulepath);
}

#endif

class Component : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~Component ();
    Adv::Component_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static Component * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual char* GetName() = 0;
    virtual void Deactivate() = 0;
    virtual void ConnectObserver( Adv::Component_ptr obs ) = 0;
    virtual void Pushed( const Adv::Event& event ) = 0;
    virtual Adv::StringSeq* GetHTML( const char* method, const Adv::NamedValueSeq& header, const Adv::NamedValueSeq& params ) = 0;

  protected:
    Component () {};

  private:
    Component (const Component &);
    void operator= (const Component &);
};

#ifdef HAVE_NAMESPACE

template<class T>
class Component_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public Component
{
  public:
    Component_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    Component_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    Component_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    Component_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~Component_tie ();

    PortableServer::POA_ptr _default_POA ();

    char* GetName();
    void Deactivate();
    void ConnectObserver( Adv::Component_ptr obs );
    void Pushed( const Adv::Event& event );
    Adv::StringSeq* GetHTML( const char* method, const Adv::NamedValueSeq& header, const Adv::NamedValueSeq& params );

  protected:
    Component_tie () {};

  private:
    Component_tie (const Component_tie<T> &);
    void operator= (const Component_tie<T> &);
};

template<class T>
Component_tie<T>::~Component_tie ()
{
}

template<class T>
PortableServer::POA_ptr
Component_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
char*
Component_tie<T>::GetName ()
{
  return POA_Tie_Base<T>::ptr->GetName ();
}

template<class T>
void
Component_tie<T>::Deactivate ()
{
  POA_Tie_Base<T>::ptr->Deactivate ();
}

template<class T>
void
Component_tie<T>::ConnectObserver (Adv::Component_ptr obs)
{
  POA_Tie_Base<T>::ptr->ConnectObserver (obs);
}

template<class T>
void
Component_tie<T>::Pushed (const Adv::Event& event)
{
  POA_Tie_Base<T>::ptr->Pushed (event);
}

template<class T>
Adv::StringSeq*
Component_tie<T>::GetHTML (const char* method, const Adv::NamedValueSeq& header, const Adv::NamedValueSeq& params)
{
  return POA_Tie_Base<T>::ptr->GetHTML (method, header, params);
}

#endif

class DocAccessor : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~DocAccessor ();
    Adv::DocAccessor_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static DocAccessor * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual char* GetDocumentID() = 0;
    virtual CORBA::Boolean GetNthProperty( CORBA::Long n, char*& key, char*& val ) = 0;
    virtual CORBA::Boolean GetProperty( const char* key, char*& val ) = 0;
    virtual void SetProperty( const char* key, const char* val ) = 0;
    virtual void UnsetNthProperty( CORBA::Long n ) = 0;
    virtual CORBA::Long GetSize() = 0;
    virtual CORBA::Long GetOctet( Adv::Int64 offset, Adv::Int32 length, Adv::OctetSeq*& buffer ) = 0;
    virtual CORBA::Long SetOctet( Adv::Int64 offset, const Adv::OctetSeq& buffer ) = 0;

  protected:
    DocAccessor () {};

  private:
    DocAccessor (const DocAccessor &);
    void operator= (const DocAccessor &);
};

#ifdef HAVE_NAMESPACE

template<class T>
class DocAccessor_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public DocAccessor
{
  public:
    DocAccessor_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    DocAccessor_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    DocAccessor_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    DocAccessor_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~DocAccessor_tie ();

    PortableServer::POA_ptr _default_POA ();

    char* GetDocumentID();
    CORBA::Boolean GetNthProperty( CORBA::Long n, char*& key, char*& val );
    CORBA::Boolean GetProperty( const char* key, char*& val );
    void SetProperty( const char* key, const char* val );
    void UnsetNthProperty( CORBA::Long n );
    CORBA::Long GetSize();
    CORBA::Long GetOctet( Adv::Int64 offset, Adv::Int32 length, Adv::OctetSeq*& buffer );
    CORBA::Long SetOctet( Adv::Int64 offset, const Adv::OctetSeq& buffer );

  protected:
    DocAccessor_tie () {};

  private:
    DocAccessor_tie (const DocAccessor_tie<T> &);
    void operator= (const DocAccessor_tie<T> &);
};

template<class T>
DocAccessor_tie<T>::~DocAccessor_tie ()
{
}

template<class T>
PortableServer::POA_ptr
DocAccessor_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
char*
DocAccessor_tie<T>::GetDocumentID ()
{
  return POA_Tie_Base<T>::ptr->GetDocumentID ();
}

template<class T>
CORBA::Boolean
DocAccessor_tie<T>::GetNthProperty (CORBA::Long n, char*& key, char*& val)
{
  return POA_Tie_Base<T>::ptr->GetNthProperty (n, key, val);
}

template<class T>
CORBA::Boolean
DocAccessor_tie<T>::GetProperty (const char* key, char*& val)
{
  return POA_Tie_Base<T>::ptr->GetProperty (key, val);
}

template<class T>
void
DocAccessor_tie<T>::SetProperty (const char* key, const char* val)
{
  POA_Tie_Base<T>::ptr->SetProperty (key, val);
}

template<class T>
void
DocAccessor_tie<T>::UnsetNthProperty (CORBA::Long n)
{
  POA_Tie_Base<T>::ptr->UnsetNthProperty (n);
}

template<class T>
CORBA::Long
DocAccessor_tie<T>::GetSize ()
{
  return POA_Tie_Base<T>::ptr->GetSize ();
}

template<class T>
CORBA::Long
DocAccessor_tie<T>::GetOctet (Adv::Int64 offset, Adv::Int32 length, Adv::OctetSeq*& buffer)
{
  return POA_Tie_Base<T>::ptr->GetOctet (offset, length, buffer);
}

template<class T>
CORBA::Long
DocAccessor_tie<T>::SetOctet (Adv::Int64 offset, const Adv::OctetSeq& buffer)
{
  return POA_Tie_Base<T>::ptr->SetOctet (offset, buffer);
}

#endif

class DocStorage : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~DocStorage ();
    Adv::DocStorage_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static DocStorage * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual CORBA::Long GetNumDocuments() = 0;
    virtual void Clear() = 0;
    virtual Adv::DocAccessor_ptr Create( const char* documentid ) = 0;
    virtual Adv::DocAccessor_ptr OpenNth( CORBA::Long n ) = 0;
    virtual Adv::DocAccessor_ptr OpenByDocumentID( const char* documentid ) = 0;
    virtual Adv::DocAccessor_ptr OpenByProperty( Adv::DocAccessor_ptr prev, const Adv::StringSeq& properties ) = 0;

  protected:
    DocStorage () {};

  private:
    DocStorage (const DocStorage &);
    void operator= (const DocStorage &);
};

#ifdef HAVE_NAMESPACE

template<class T>
class DocStorage_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public DocStorage
{
  public:
    DocStorage_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    DocStorage_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    DocStorage_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    DocStorage_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~DocStorage_tie ();

    PortableServer::POA_ptr _default_POA ();

    CORBA::Long GetNumDocuments();
    void Clear();
    Adv::DocAccessor_ptr Create( const char* documentid );
    Adv::DocAccessor_ptr OpenNth( CORBA::Long n );
    Adv::DocAccessor_ptr OpenByDocumentID( const char* documentid );
    Adv::DocAccessor_ptr OpenByProperty( Adv::DocAccessor_ptr prev, const Adv::StringSeq& properties );

  protected:
    DocStorage_tie () {};

  private:
    DocStorage_tie (const DocStorage_tie<T> &);
    void operator= (const DocStorage_tie<T> &);
};

template<class T>
DocStorage_tie<T>::~DocStorage_tie ()
{
}

template<class T>
PortableServer::POA_ptr
DocStorage_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
CORBA::Long
DocStorage_tie<T>::GetNumDocuments ()
{
  return POA_Tie_Base<T>::ptr->GetNumDocuments ();
}

template<class T>
void
DocStorage_tie<T>::Clear ()
{
  POA_Tie_Base<T>::ptr->Clear ();
}

template<class T>
Adv::DocAccessor_ptr
DocStorage_tie<T>::Create (const char* documentid)
{
  return POA_Tie_Base<T>::ptr->Create (documentid);
}

template<class T>
Adv::DocAccessor_ptr
DocStorage_tie<T>::OpenNth (CORBA::Long n)
{
  return POA_Tie_Base<T>::ptr->OpenNth (n);
}

template<class T>
Adv::DocAccessor_ptr
DocStorage_tie<T>::OpenByDocumentID (const char* documentid)
{
  return POA_Tie_Base<T>::ptr->OpenByDocumentID (documentid);
}

template<class T>
Adv::DocAccessor_ptr
DocStorage_tie<T>::OpenByProperty (Adv::DocAccessor_ptr prev, const Adv::StringSeq& properties)
{
  return POA_Tie_Base<T>::ptr->OpenByProperty (prev, properties);
}

#endif

class Container : 
  virtual public POA_Adv::Component
{
  public:
    virtual ~Container ();
    Adv::Container_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static Container * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

  protected:
    Container () {};

  private:
    Container (const Container &);
    void operator= (const Container &);
};

#ifdef HAVE_NAMESPACE

template<class T>
class Container_tie : 
  virtual public POA_Adv::Component_tie<T>,
  virtual public Container
{
  public:
    Container_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    Container_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    Container_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    Container_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~Container_tie ();

    PortableServer::POA_ptr _default_POA ();

  protected:
    Container_tie () {};

  private:
    Container_tie (const Container_tie<T> &);
    void operator= (const Container_tie<T> &);
};

template<class T>
Container_tie<T>::~Container_tie ()
{
}

template<class T>
PortableServer::POA_ptr
Container_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

#endif

#endif // !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE_POA_Adv)

#ifndef MICO_NO_TOPLEVEL_MODULES

};
#endif



#ifndef HAVE_NAMESPACE

template<class T>
class POA_Adv_SessionManager_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public POA_Adv::SessionManager
{
  public:
    POA_Adv_SessionManager_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    POA_Adv_SessionManager_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    POA_Adv_SessionManager_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    POA_Adv_SessionManager_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~POA_Adv_SessionManager_tie ();

    PortableServer::POA_ptr _default_POA ();

    char* GetUserName();
    Adv::HostManagerSeq* GetHostManagers();
    char* GetLocation();
    void ResistComponent( Adv::Component_ptr component );
    Adv::StringSeq* GetHTML( const char* method, const char* object, const Adv::NamedValueSeq& header, const Adv::NamedValueSeq& params );

  protected:
    POA_Adv_SessionManager_tie () {};

  private:
    POA_Adv_SessionManager_tie (const POA_Adv_SessionManager_tie<T> &);
    void operator= (const POA_Adv_SessionManager_tie<T> &);
};

template<class T>
POA_Adv_SessionManager_tie<T>::~POA_Adv_SessionManager_tie ()
{
}

template<class T>
PortableServer::POA_ptr
POA_Adv_SessionManager_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
char*
POA_Adv_SessionManager_tie<T>::GetUserName ()
{
  return POA_Tie_Base<T>::ptr->GetUserName ();
}

template<class T>
Adv::HostManagerSeq*
POA_Adv_SessionManager_tie<T>::GetHostManagers ()
{
  return POA_Tie_Base<T>::ptr->GetHostManagers ();
}

template<class T>
char*
POA_Adv_SessionManager_tie<T>::GetLocation ()
{
  return POA_Tie_Base<T>::ptr->GetLocation ();
}

template<class T>
void
POA_Adv_SessionManager_tie<T>::ResistComponent (Adv::Component_ptr component)
{
  POA_Tie_Base<T>::ptr->ResistComponent (component);
}

template<class T>
Adv::StringSeq*
POA_Adv_SessionManager_tie<T>::GetHTML (const char* method, const char* object, const Adv::NamedValueSeq& header, const Adv::NamedValueSeq& params)
{
  return POA_Tie_Base<T>::ptr->GetHTML (method, object, header, params);
}

#endif

#ifndef HAVE_NAMESPACE

template<class T>
class POA_Adv_ClusterManager_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public POA_Adv::ClusterManager
{
  public:
    POA_Adv_ClusterManager_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    POA_Adv_ClusterManager_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    POA_Adv_ClusterManager_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    POA_Adv_ClusterManager_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~POA_Adv_ClusterManager_tie ();

    PortableServer::POA_ptr _default_POA ();

    void ResistHostManager( Adv::HostManager_ptr hostmanager );
    Adv::HostManagerSeq* GetHostManagers();

  protected:
    POA_Adv_ClusterManager_tie () {};

  private:
    POA_Adv_ClusterManager_tie (const POA_Adv_ClusterManager_tie<T> &);
    void operator= (const POA_Adv_ClusterManager_tie<T> &);
};

template<class T>
POA_Adv_ClusterManager_tie<T>::~POA_Adv_ClusterManager_tie ()
{
}

template<class T>
PortableServer::POA_ptr
POA_Adv_ClusterManager_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
void
POA_Adv_ClusterManager_tie<T>::ResistHostManager (Adv::HostManager_ptr hostmanager)
{
  POA_Tie_Base<T>::ptr->ResistHostManager (hostmanager);
}

template<class T>
Adv::HostManagerSeq*
POA_Adv_ClusterManager_tie<T>::GetHostManagers ()
{
  return POA_Tie_Base<T>::ptr->GetHostManagers ();
}

#endif

#ifndef HAVE_NAMESPACE

template<class T>
class POA_Adv_HostManager_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public POA_Adv::HostManager
{
  public:
    POA_Adv_HostManager_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    POA_Adv_HostManager_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    POA_Adv_HostManager_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    POA_Adv_HostManager_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~POA_Adv_HostManager_tie ();

    PortableServer::POA_ptr _default_POA ();

    char* GetHostName();
    char* GetHostIP();
    Adv::HostState GetHostState();
    Adv::ProcessFactory_ptr GetProcessFactory();
    Adv::DocumentManager_ptr GetDocumentManager();

  protected:
    POA_Adv_HostManager_tie () {};

  private:
    POA_Adv_HostManager_tie (const POA_Adv_HostManager_tie<T> &);
    void operator= (const POA_Adv_HostManager_tie<T> &);
};

template<class T>
POA_Adv_HostManager_tie<T>::~POA_Adv_HostManager_tie ()
{
}

template<class T>
PortableServer::POA_ptr
POA_Adv_HostManager_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
char*
POA_Adv_HostManager_tie<T>::GetHostName ()
{
  return POA_Tie_Base<T>::ptr->GetHostName ();
}

template<class T>
char*
POA_Adv_HostManager_tie<T>::GetHostIP ()
{
  return POA_Tie_Base<T>::ptr->GetHostIP ();
}

template<class T>
Adv::HostState
POA_Adv_HostManager_tie<T>::GetHostState ()
{
  return POA_Tie_Base<T>::ptr->GetHostState ();
}

template<class T>
Adv::ProcessFactory_ptr
POA_Adv_HostManager_tie<T>::GetProcessFactory ()
{
  return POA_Tie_Base<T>::ptr->GetProcessFactory ();
}

template<class T>
Adv::DocumentManager_ptr
POA_Adv_HostManager_tie<T>::GetDocumentManager ()
{
  return POA_Tie_Base<T>::ptr->GetDocumentManager ();
}

#endif

#ifndef HAVE_NAMESPACE

template<class T>
class POA_Adv_DocumentManager_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public POA_Adv::DocumentManager
{
  public:
    POA_Adv_DocumentManager_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    POA_Adv_DocumentManager_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    POA_Adv_DocumentManager_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    POA_Adv_DocumentManager_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~POA_Adv_DocumentManager_tie ();

    PortableServer::POA_ptr _default_POA ();

    char* Export( const char* locator );
    char* Unexport( const char* locator );
    Adv::StringSeq* GetList();
    char* QueryDocumentID( const char* id );
    char* QueryLocator( const char* locator );

  protected:
    POA_Adv_DocumentManager_tie () {};

  private:
    POA_Adv_DocumentManager_tie (const POA_Adv_DocumentManager_tie<T> &);
    void operator= (const POA_Adv_DocumentManager_tie<T> &);
};

template<class T>
POA_Adv_DocumentManager_tie<T>::~POA_Adv_DocumentManager_tie ()
{
}

template<class T>
PortableServer::POA_ptr
POA_Adv_DocumentManager_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
char*
POA_Adv_DocumentManager_tie<T>::Export (const char* locator)
{
  return POA_Tie_Base<T>::ptr->Export (locator);
}

template<class T>
char*
POA_Adv_DocumentManager_tie<T>::Unexport (const char* locator)
{
  return POA_Tie_Base<T>::ptr->Unexport (locator);
}

template<class T>
Adv::StringSeq*
POA_Adv_DocumentManager_tie<T>::GetList ()
{
  return POA_Tie_Base<T>::ptr->GetList ();
}

template<class T>
char*
POA_Adv_DocumentManager_tie<T>::QueryDocumentID (const char* id)
{
  return POA_Tie_Base<T>::ptr->QueryDocumentID (id);
}

template<class T>
char*
POA_Adv_DocumentManager_tie<T>::QueryLocator (const char* locator)
{
  return POA_Tie_Base<T>::ptr->QueryLocator (locator);
}

#endif

#ifndef HAVE_NAMESPACE

template<class T>
class POA_Adv_ProcessFactory_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public POA_Adv::ProcessFactory
{
  public:
    POA_Adv_ProcessFactory_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    POA_Adv_ProcessFactory_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    POA_Adv_ProcessFactory_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    POA_Adv_ProcessFactory_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~POA_Adv_ProcessFactory_tie ();

    PortableServer::POA_ptr _default_POA ();

    void Execute( const char* modulepath );

  protected:
    POA_Adv_ProcessFactory_tie () {};

  private:
    POA_Adv_ProcessFactory_tie (const POA_Adv_ProcessFactory_tie<T> &);
    void operator= (const POA_Adv_ProcessFactory_tie<T> &);
};

template<class T>
POA_Adv_ProcessFactory_tie<T>::~POA_Adv_ProcessFactory_tie ()
{
}

template<class T>
PortableServer::POA_ptr
POA_Adv_ProcessFactory_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
void
POA_Adv_ProcessFactory_tie<T>::Execute (const char* modulepath)
{
  POA_Tie_Base<T>::ptr->Execute (modulepath);
}

#endif

#ifndef HAVE_NAMESPACE

template<class T>
class POA_Adv_Component_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public POA_Adv::Component
{
  public:
    POA_Adv_Component_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    POA_Adv_Component_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    POA_Adv_Component_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    POA_Adv_Component_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~POA_Adv_Component_tie ();

    PortableServer::POA_ptr _default_POA ();

    char* GetName();
    void Deactivate();
    void ConnectObserver( Adv::Component_ptr obs );
    void Pushed( const Adv::Event& event );
    Adv::StringSeq* GetHTML( const char* method, const Adv::NamedValueSeq& header, const Adv::NamedValueSeq& params );

  protected:
    POA_Adv_Component_tie () {};

  private:
    POA_Adv_Component_tie (const POA_Adv_Component_tie<T> &);
    void operator= (const POA_Adv_Component_tie<T> &);
};

template<class T>
POA_Adv_Component_tie<T>::~POA_Adv_Component_tie ()
{
}

template<class T>
PortableServer::POA_ptr
POA_Adv_Component_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
char*
POA_Adv_Component_tie<T>::GetName ()
{
  return POA_Tie_Base<T>::ptr->GetName ();
}

template<class T>
void
POA_Adv_Component_tie<T>::Deactivate ()
{
  POA_Tie_Base<T>::ptr->Deactivate ();
}

template<class T>
void
POA_Adv_Component_tie<T>::ConnectObserver (Adv::Component_ptr obs)
{
  POA_Tie_Base<T>::ptr->ConnectObserver (obs);
}

template<class T>
void
POA_Adv_Component_tie<T>::Pushed (const Adv::Event& event)
{
  POA_Tie_Base<T>::ptr->Pushed (event);
}

template<class T>
Adv::StringSeq*
POA_Adv_Component_tie<T>::GetHTML (const char* method, const Adv::NamedValueSeq& header, const Adv::NamedValueSeq& params)
{
  return POA_Tie_Base<T>::ptr->GetHTML (method, header, params);
}

#endif

#ifndef HAVE_NAMESPACE

template<class T>
class POA_Adv_DocAccessor_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public POA_Adv::DocAccessor
{
  public:
    POA_Adv_DocAccessor_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    POA_Adv_DocAccessor_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    POA_Adv_DocAccessor_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    POA_Adv_DocAccessor_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~POA_Adv_DocAccessor_tie ();

    PortableServer::POA_ptr _default_POA ();

    char* GetDocumentID();
    CORBA::Boolean GetNthProperty( CORBA::Long n, char*& key, char*& val );
    CORBA::Boolean GetProperty( const char* key, char*& val );
    void SetProperty( const char* key, const char* val );
    void UnsetNthProperty( CORBA::Long n );
    CORBA::Long GetSize();
    CORBA::Long GetOctet( Adv::Int64 offset, Adv::Int32 length, Adv::OctetSeq*& buffer );
    CORBA::Long SetOctet( Adv::Int64 offset, const Adv::OctetSeq& buffer );

  protected:
    POA_Adv_DocAccessor_tie () {};

  private:
    POA_Adv_DocAccessor_tie (const POA_Adv_DocAccessor_tie<T> &);
    void operator= (const POA_Adv_DocAccessor_tie<T> &);
};

template<class T>
POA_Adv_DocAccessor_tie<T>::~POA_Adv_DocAccessor_tie ()
{
}

template<class T>
PortableServer::POA_ptr
POA_Adv_DocAccessor_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
char*
POA_Adv_DocAccessor_tie<T>::GetDocumentID ()
{
  return POA_Tie_Base<T>::ptr->GetDocumentID ();
}

template<class T>
CORBA::Boolean
POA_Adv_DocAccessor_tie<T>::GetNthProperty (CORBA::Long n, char*& key, char*& val)
{
  return POA_Tie_Base<T>::ptr->GetNthProperty (n, key, val);
}

template<class T>
CORBA::Boolean
POA_Adv_DocAccessor_tie<T>::GetProperty (const char* key, char*& val)
{
  return POA_Tie_Base<T>::ptr->GetProperty (key, val);
}

template<class T>
void
POA_Adv_DocAccessor_tie<T>::SetProperty (const char* key, const char* val)
{
  POA_Tie_Base<T>::ptr->SetProperty (key, val);
}

template<class T>
void
POA_Adv_DocAccessor_tie<T>::UnsetNthProperty (CORBA::Long n)
{
  POA_Tie_Base<T>::ptr->UnsetNthProperty (n);
}

template<class T>
CORBA::Long
POA_Adv_DocAccessor_tie<T>::GetSize ()
{
  return POA_Tie_Base<T>::ptr->GetSize ();
}

template<class T>
CORBA::Long
POA_Adv_DocAccessor_tie<T>::GetOctet (Adv::Int64 offset, Adv::Int32 length, Adv::OctetSeq*& buffer)
{
  return POA_Tie_Base<T>::ptr->GetOctet (offset, length, buffer);
}

template<class T>
CORBA::Long
POA_Adv_DocAccessor_tie<T>::SetOctet (Adv::Int64 offset, const Adv::OctetSeq& buffer)
{
  return POA_Tie_Base<T>::ptr->SetOctet (offset, buffer);
}

#endif

#ifndef HAVE_NAMESPACE

template<class T>
class POA_Adv_DocStorage_tie : 
  virtual public POA_Tie_Base<T>,
  virtual public POA_Adv::DocStorage
{
  public:
    POA_Adv_DocStorage_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    POA_Adv_DocStorage_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    POA_Adv_DocStorage_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    POA_Adv_DocStorage_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~POA_Adv_DocStorage_tie ();

    PortableServer::POA_ptr _default_POA ();

    CORBA::Long GetNumDocuments();
    void Clear();
    Adv::DocAccessor_ptr Create( const char* documentid );
    Adv::DocAccessor_ptr OpenNth( CORBA::Long n );
    Adv::DocAccessor_ptr OpenByDocumentID( const char* documentid );
    Adv::DocAccessor_ptr OpenByProperty( Adv::DocAccessor_ptr prev, const Adv::StringSeq& properties );

  protected:
    POA_Adv_DocStorage_tie () {};

  private:
    POA_Adv_DocStorage_tie (const POA_Adv_DocStorage_tie<T> &);
    void operator= (const POA_Adv_DocStorage_tie<T> &);
};

template<class T>
POA_Adv_DocStorage_tie<T>::~POA_Adv_DocStorage_tie ()
{
}

template<class T>
PortableServer::POA_ptr
POA_Adv_DocStorage_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

template<class T>
CORBA::Long
POA_Adv_DocStorage_tie<T>::GetNumDocuments ()
{
  return POA_Tie_Base<T>::ptr->GetNumDocuments ();
}

template<class T>
void
POA_Adv_DocStorage_tie<T>::Clear ()
{
  POA_Tie_Base<T>::ptr->Clear ();
}

template<class T>
Adv::DocAccessor_ptr
POA_Adv_DocStorage_tie<T>::Create (const char* documentid)
{
  return POA_Tie_Base<T>::ptr->Create (documentid);
}

template<class T>
Adv::DocAccessor_ptr
POA_Adv_DocStorage_tie<T>::OpenNth (CORBA::Long n)
{
  return POA_Tie_Base<T>::ptr->OpenNth (n);
}

template<class T>
Adv::DocAccessor_ptr
POA_Adv_DocStorage_tie<T>::OpenByDocumentID (const char* documentid)
{
  return POA_Tie_Base<T>::ptr->OpenByDocumentID (documentid);
}

template<class T>
Adv::DocAccessor_ptr
POA_Adv_DocStorage_tie<T>::OpenByProperty (Adv::DocAccessor_ptr prev, const Adv::StringSeq& properties)
{
  return POA_Tie_Base<T>::ptr->OpenByProperty (prev, properties);
}

#endif

#ifndef HAVE_NAMESPACE

template<class T>
class POA_Adv_Container_tie : 
  virtual public POA_Adv_Component_tie<T>,
  virtual public POA_Adv::Container
{
  public:
    POA_Adv_Container_tie (T &t)
      : POA_Tie_Base<T> (&t, PortableServer::POA::_nil(), FALSE)
    {}
    POA_Adv_Container_tie (T &t, PortableServer::POA_ptr _poa)
      : POA_Tie_Base<T> (&t, _poa, FALSE)
    {}
    POA_Adv_Container_tie (T *t, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, PortableServer::POA::_nil(), _rel)
    {}
    POA_Adv_Container_tie (T *t, PortableServer::POA_ptr _poa, CORBA::Boolean _rel = TRUE)
      : POA_Tie_Base<T> (t, _poa, _rel)
    {}
    virtual ~POA_Adv_Container_tie ();

    PortableServer::POA_ptr _default_POA ();

  protected:
    POA_Adv_Container_tie () {};

  private:
    POA_Adv_Container_tie (const POA_Adv_Container_tie<T> &);
    void operator= (const POA_Adv_Container_tie<T> &);
};

template<class T>
POA_Adv_Container_tie<T>::~POA_Adv_Container_tie ()
{
}

template<class T>
PortableServer::POA_ptr
POA_Adv_Container_tie<T>::_default_POA ()
{
  if (!CORBA::is_nil (POA_Tie_Base<T>::poa)) {
    return PortableServer::POA::_duplicate (POA_Tie_Base<T>::poa);
  }
  return PortableServer::ServantBase::_default_POA ();
}

#endif

#endif // MICO_CONF_NO_POA

#if !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE__GLOBAL)

extern CORBA::StaticTypeInfo *_marshaller_Adv_Event;

extern CORBA::StaticTypeInfo *_marshaller_Adv_HostState;

extern CORBA::StaticTypeInfo *_marshaller_Adv_NamedValue;

extern CORBA::StaticTypeInfo *_marshaller_Adv_SessionManager_NotFound;

extern CORBA::StaticTypeInfo *_marshaller_Adv_SessionManager;

extern CORBA::StaticTypeInfo *_marshaller_Adv_ClusterManager;

extern CORBA::StaticTypeInfo *_marshaller_Adv_HostManager;

extern CORBA::StaticTypeInfo *_marshaller_Adv_DocumentManager_CannotFind;

extern CORBA::StaticTypeInfo *_marshaller_Adv_DocumentManager;

extern CORBA::StaticTypeInfo *_marshaller_Adv_ProcessFactory;

extern CORBA::StaticTypeInfo *_marshaller_Adv_Component;

extern CORBA::StaticTypeInfo *_marshaller_Adv_DocAccessor;

extern CORBA::StaticTypeInfo *_marshaller_Adv_DocStorage_NoSuchDocument;

extern CORBA::StaticTypeInfo *_marshaller_Adv_DocStorage_NotEnoughMemory;

extern CORBA::StaticTypeInfo *_marshaller_Adv_DocStorage;

extern CORBA::StaticTypeInfo *_marshaller_Adv_Container;

extern CORBA::StaticTypeInfo *_marshaller__seq_Adv_HostManager;

extern CORBA::StaticTypeInfo *_marshaller__a3_float;

extern CORBA::StaticTypeInfo *_marshaller__seq_Adv_NamedValue;

#endif // !defined(MICO_NO_TOPLEVEL_MODULES) || defined(MICO_MODULE__GLOBAL)


#if !defined(MICO_NO_TOPLEVEL_MODULES) && !defined(MICO_IN_GENERATED_CODE)
#include <mico/template_impl.h>
#endif

#endif
