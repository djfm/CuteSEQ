#ifndef UTIL_H
#define UTIL_H

template <class T>
class Maybe
{

    T _value;
    bool _nothing;

public:

    Maybe() : _nothing(true)
    {
    }

    Maybe(const T & value) : _value(value), _nothing(false)
    {
    }

    static Maybe Nothing()
    {
        return Maybe();
    }

    static Maybe Just(const T & value)
    {
        return Maybe(value);
    }

    T & value()
    {
        return _value;
    }

    const T & value() const
    {
        return _value;
    }

    bool just() const
    {
        return not _nothing;
    }

    bool nothing() const
    {
        return _nothing;
    }

    operator bool() const
    {
        return just();
    }

    bool operator==(const Maybe & other) const
    {
        if(other.just() and just())
        {
            return other.value() == value();
        }
        else if(other.nothing() and nothing())return true;
        else return false;
    }

    bool operator!=(const Maybe & other) const
    {
        return not operator==(other);
    }

    void clear()
    {
        *this = Nothing();
    }

};

#endif // UTIL_H
