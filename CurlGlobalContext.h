#ifndef APITESTER_CURLGLOBALCONTEXT_H
#define APITESTER_CURLGLOBALCONTEXT_H

/*
 * A RAII wrapper around curl global state initialization.
 */
class CurlGlobalContext {
public:
    CurlGlobalContext();
    ~CurlGlobalContext();
};


#endif //APITESTER_CURLGLOBALCONTEXT_H