#ifndef JUNO_DETECTOR_IDENTIFIER_H
#define JUNO_DETECTOR_IDENTIFIER_H

//
//  Identifier is a simple type-safe 32 bit unsigned integer. An
//  Identifier relies on other classes to encode and
//  decode its information.
//  
//  The default constructor created an Identifier an invalid state
//  which can be check with the "is_valid" method to allow some error
//  checking.
//
//  Author: Zhengyun You  2013-11-20
//

#include <vector>
#include <string>

class Identifier
{
    public:

        ///----------------------------------------------------------------
        /// Define public typedefs
        ///----------------------------------------------------------------
        typedef Identifier   id_type;
        typedef unsigned int value_type;
        typedef unsigned int size_type;

        ///----------------------------------------------------------------
        /// Constructors
        ///----------------------------------------------------------------

        /// Default constructor
        Identifier ();

        /// Constructor from value_type
        explicit Identifier (value_type value);

        /// Copy constructor
        Identifier (const Identifier& other);

        ///----------------------------------------------------------------
        /// Modifications
        ///----------------------------------------------------------------

        /// Assignment operator
        Identifier& operator = (value_type value);
        Identifier& operator = (const Identifier& value) = default;

        /// Bitwise operations 
        Identifier& operator |= (value_type value);
        Identifier& operator &= (value_type value);

        /// build from a string form - hexadecimal
        void set (const std::string& id);

        /// Reset to invalid state
        void clear ();

        ///----------------------------------------------------------------
        /// Accessors
        ///----------------------------------------------------------------
        /// Get the value 
        operator value_type         (void) const;
        value_type  getValue() const;

        ///----------------------------------------------------------------
        /// Comparison operators
        ///----------------------------------------------------------------
        bool operator ==    (const Identifier& other) const;
        bool operator !=    (const Identifier& other) const;
        bool operator <     (const Identifier& other) const;
        bool operator >     (const Identifier& other) const;

        ///----------------------------------------------------------------
        /// Error management
        ///----------------------------------------------------------------

        /// Check if id is in a valid state
        bool isValid () const;
  
        ///----------------------------------------------------------------
        /// Utilities
        ///----------------------------------------------------------------

        /// Provide a string form of the identifier - hexadecimal
        std::string  getString() const;

        /// Print out in hex form
        void show () const;

    private:

        typedef enum {
          maxValue = 0xFFFFFFFF
        } maxvalue_type;

        //----------------------------------------------------------------
        // The compact identifier data.
        //----------------------------------------------------------------
        value_type m_id;
};

//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

// Constructors
//-----------------------------------------------
inline Identifier::Identifier ()
    : m_id(maxValue)
{
}

inline Identifier::Identifier (const Identifier& other)
    : m_id(other.getValue())
{
}

inline Identifier::Identifier (value_type value)
    : m_id(value)
{
}

// Modifications
//-----------------------------------------------

inline Identifier&
Identifier::operator = (value_type value)
{
    m_id = value;
    return (*this);
}

inline Identifier&                                   
Identifier::operator |= (unsigned int value)
{
    m_id |= value;
    return (*this);
}

inline Identifier& 
Identifier::operator &= (unsigned int value)
{
    m_id &= value;
    return (*this);
}

inline void 
Identifier::clear () 
{
    m_id = maxValue;
}

// Accessors
//----------------------------------------------------------------
inline Identifier::operator Identifier::value_type (void) const
{
    return (m_id);
}
                                             
inline Identifier::value_type Identifier::getValue() const
{
    return (m_id);
}

// Comparison operators
//----------------------------------------------------------------
inline bool 
Identifier::operator == (const Identifier& other) const
{
    return (m_id == other.getValue());
}

inline bool 
Identifier::operator != (const Identifier& other) const
{
    return (m_id != other.getValue());
}

inline bool 
Identifier::operator < (const Identifier& other) const
{
    return (m_id < other.getValue());
}

inline bool 
Identifier::operator > (const Identifier& other) const
{
    return (m_id > other.getValue());
}

inline bool 
Identifier::isValid () const
{
    return (!(maxValue == m_id));
}

// Others 
//----------------------------------------------------------------
std::ostream& operator<<(std::ostream & os, const Identifier& Id);

#endif // JUNO_DETECTOR_IDENTIFIER_H

