/*
 * errors.h
 */

#ifndef __ERROR_H
#define __ERROR_H

#include <exception>
#include <string>

using namespace std;

class UnboundVariableException : public exception {
    string reason;
 public:
    UnboundVariableException(string var) {
        reason = "Unbounded Variable " + var;
    };

    const char * what() { return reason.c_str(); };
};

class NoContextException : public exception {
 public:
    NoContextException() { };
    const char * what() { return "No Context"; };
};

class NotABeliefException : public exception {
 public:
    NotABeliefException() { };
    const char * what() { return "Not a belief"; };
};

class NotAProcedureException : public exception {
 public:
    NotAProcedureException() { };
    const char * what() { return "Not a procedure"; };
};

class CutPlanException : public exception {
 public:
    CutPlanException() { };
    const char * what() { return "Plan Cut"; };
};

class AgentNotFoundException : public exception {
    string reason;
 public:
    AgentNotFoundException(string var) {
        reason = "Agent " + var + " not found";
    };

    const char * what() { return reason.c_str(); };
};

class MessageNotAcceptedException : public exception {
 public:
    MessageNotAcceptedException() { };
    const char * what() { return "The message cannot be accepted"; };
};

class BadMessageSyntaxException : public exception {
    string reason;
 public:
    BadMessageSyntaxException(string var) {
        reason = var + ": BadMessageSyntaxException";
    };

    const char * what() { return reason.c_str(); };
};

class ProtocolNotStartedException : public exception {
 public:
    ProtocolNotStartedException() { };
    const char * what() { return "Protocol not started"; };
};

class InvalidMagicNumber : public exception {
 public:
    InvalidMagicNumber() { };
    const char * what() { return "Invalid packet magic number"; };
};

#endif
