#include "./include/Ease.h"
#include "./include/Bezier.h"

const flt::ease::EaseFunction flt::ease::linear = [](float t){return t;};
const flt::ease::EaseFunction flt::ease::ease = [](float t){return t * t * (3.0f - 2.0f * t);};
const flt::ease::EaseFunction flt::ease::easeIn = [](float t){return t * t;};
const flt::ease::EaseFunction flt::ease::easeOut = [](float t){return t * (2.0f - t);};
const flt::ease::EaseFunction flt::ease::easeInOut = [](float t){return t * t * (3.0f - 2.0f * t);};

const flt::ease::EaseFunction flt::ease::bezierEase = [](float t) { return flt::Bezier::Ease()(t); };
const flt::ease::EaseFunction flt::ease::bezierEaseIn = [](float t) { return flt::Bezier::EaseIn()(t); };
const flt::ease::EaseFunction flt::ease::bezierEaseOut = [](float t) { return flt::Bezier::EaseOut()(t); };
const flt::ease::EaseFunction flt::ease::bezierEaseInOut = [](float t) { return flt::Bezier::EaseInOut()(t); };
