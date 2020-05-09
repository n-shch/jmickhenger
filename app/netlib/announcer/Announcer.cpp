#include "Announcer.h"

using namespace std;
using namespace inf;

Announcer::Announcer() {
    generalCallback = nullopt;
}

Announcer::~Announcer() {
    generalCallback = nullopt;
    callbacks.clear();
}

void Announcer::addCallback(int idChat, const function<void(inf::ChatChange &)> &callback) {
    callbacks[idChat] = callback;
}

void Announcer::setGeneralCallback(const function<void(inf::ChatChange &)> &callback) {
    generalCallback = callback;
}

void Announcer::notify(inf::ChatChange &change) {
    if(callbacks.count(change.idChat)) {
        callbacks[change.idChat](change);
    } else if(generalCallback) {
        generalCallback.value()(change);
    }
}