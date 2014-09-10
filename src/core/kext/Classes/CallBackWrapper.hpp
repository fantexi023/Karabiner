#ifndef CALLBACKWRAPPER_HPP
#define CALLBACKWRAPPER_HPP

#include "CommonData.hpp"
#include "IOLogWrapper.hpp"
#include "KeyCode.hpp"
#include "Types.hpp"
#include "auto_ptr.hpp"

namespace org_pqrs_Karabiner {
  class Params_KeyboardEventCallBack {
    friend class EventOutputQueue;

  public:
    Params_KeyboardEventCallBack(EventType et, Flags fl, KeyCode kc,
                                 CharCode cc, CharSet cs, OrigCharCode occ, OrigCharSet ocs,
                                 KeyboardType kt, bool r) :
      eventType(et), flags(fl), key(kc),
      charCode(cc), charSet(cs), origCharCode(occ), origCharSet(ocs),
      keyboardType(kt), repeat(r),
      ex_iskeydown(et.isKeyDownOrModifierDown(kc, fl))
    {
      CommonData::increase_alloccount();
    }

    Params_KeyboardEventCallBack(EventType et, Flags fl, KeyCode kc,
                                 KeyboardType kt, bool r) :
      eventType(et), flags(fl), key(kc),
      charCode(0), charSet(0), origCharCode(0), origCharSet(0),
      keyboardType(kt), repeat(r),
      ex_iskeydown(et.isKeyDownOrModifierDown(kc, fl))
    {
      CommonData::increase_alloccount();
    }

    ~Params_KeyboardEventCallBack(void) {
      CommonData::decrease_alloccount();
    }

    // Use auto_ptr instead allocating in kernel stack. (Reduce kernel stack usage.)
    DECLARE_AUTO_PTR(Params_KeyboardEventCallBack);

    static Params_KeyboardEventCallBack* alloc(const Params_KeyboardEventCallBack& p) {
      return new Params_KeyboardEventCallBack(p.eventType, p.flags, p.key,
                                              p.charCode, p.charSet, p.origCharCode, p.origCharSet,
                                              p.keyboardType, p.repeat);
    }

    // ----------------------------------------
    static void log(bool isCaught, EventType eventType, Flags flags, KeyCode key, KeyboardType keyboardType, bool repeat) {
      IOLOG_DEBUG("KeyboardEventCallback [%7s]: eventType %2d, flags 0x%08x, key 0x%04x, kbdType %3d, repeat = %d\n",
                  isCaught ? "caught" : "sending",
                  eventType.get(), flags.get(), key.get(), keyboardType.get(), repeat);
    }

    const EventType eventType;
    const Flags flags;
    const KeyCode key;
    const CharCode charCode;
    const CharSet charSet;
    const OrigCharCode origCharCode;
    const OrigCharSet origCharSet;
    const KeyboardType keyboardType;
    const bool repeat;

    const bool ex_iskeydown;
  };

  class Params_UpdateEventFlagsCallback {
    friend class EventOutputQueue;

  public:
    Params_UpdateEventFlagsCallback(Flags fl) : flags(fl) {
      CommonData::increase_alloccount();
    }

    ~Params_UpdateEventFlagsCallback(void) {
      CommonData::decrease_alloccount();
    }

    // Use auto_ptr instead allocating in kernel stack. (Reduce kernel stack usage.)
    DECLARE_AUTO_PTR(Params_UpdateEventFlagsCallback);

    static Params_UpdateEventFlagsCallback* alloc(const Params_UpdateEventFlagsCallback& p) {
      return new Params_UpdateEventFlagsCallback(p.flags);
    }

    // ----------------------------------------
    static void log(bool isCaught, Flags flags) {
      IOLOG_DEBUG("UpdateEventFlagsCallback [%7s]: flags 0x%08x\n",
                  isCaught ? "caught" : "sending",
                  flags.get());
    }

    const Flags flags;
  };

  class Params_KeyboardSpecialEventCallback {
    friend class EventOutputQueue;

  public:
    Params_KeyboardSpecialEventCallback(EventType et, Flags fl, ConsumerKeyCode ckc,
                                        unsigned int fv, UInt64 g,
                                        bool r) :
      eventType(et), flags(fl), key(ckc),
      flavor(fv), guid(g),
      repeat(r), ex_iskeydown(et == EventType::DOWN)
    {
      CommonData::increase_alloccount();
    }

    Params_KeyboardSpecialEventCallback(EventType et, Flags fl, ConsumerKeyCode ckc,
                                        bool r) :
      eventType(et), flags(fl), key(ckc),
      flavor(ckc.get()), guid(static_cast<UInt64>(-1)),
      repeat(r), ex_iskeydown(et == EventType::DOWN)
    {
      CommonData::increase_alloccount();
    }

    ~Params_KeyboardSpecialEventCallback(void) {
      CommonData::decrease_alloccount();
    }

    // Use auto_ptr instead allocating in kernel stack. (Reduce kernel stack usage.)
    DECLARE_AUTO_PTR(Params_KeyboardSpecialEventCallback);

    static Params_KeyboardSpecialEventCallback* alloc(const Params_KeyboardSpecialEventCallback& p) {
      return new Params_KeyboardSpecialEventCallback(p.eventType,
                                                     p.flags,
                                                     p.key,
                                                     p.flavor,
                                                     p.guid,
                                                     p.repeat);
    }

    // ----------------------------------------
    static void log(bool isCaught, EventType eventType, Flags flags, ConsumerKeyCode key, unsigned int flavor, UInt64 guid, bool repeat) {
      IOLOG_DEBUG("KeyboardSpecialEventCallBack [%7s]: eventType %2d, flags 0x%08x, key 0x%04x, flavor %4d, guid %lld, repeat = %d\n",
                  isCaught ? "caught" : "sending",
                  eventType.get(), flags.get(), key.get(), flavor, guid, repeat);
    }

    const EventType eventType;
    const Flags flags;
    const ConsumerKeyCode key;
    const unsigned int flavor;
    const UInt64 guid;
    const bool repeat;

    const bool ex_iskeydown;
  };

  class Params_RelativePointerEventCallback {
    friend class EventOutputQueue;

  public:
    ~Params_RelativePointerEventCallback(void) {
      CommonData::decrease_alloccount();
    }

    // Use auto_ptr instead allocating in kernel stack. (Reduce kernel stack usage.)
    DECLARE_AUTO_PTR(Params_RelativePointerEventCallback);

    static Params_RelativePointerEventCallback* alloc(Buttons bt, int x, int y, PointingButton ex_btn, bool ex_isdown) {
      return new Params_RelativePointerEventCallback(bt, x, y, ex_btn, ex_isdown);
    }

    static Params_RelativePointerEventCallback* alloc(const Params_RelativePointerEventCallback& p) {
      return new Params_RelativePointerEventCallback(p.buttons, p.dx, p.dy, p.ex_button, p.ex_isbuttondown);
    }

    static void log(bool isCaught, Buttons buttons, int dx, int dy) {
      IOLOG_DEBUG_POINTING("RelativePointerEventCallBack [%7s]: buttons: 0x%08x, dx: %3d, dy: %3d\n",
                           isCaught ? "caught" : "sending",
                           buttons.get(), dx, dy);
    }

    const Buttons buttons;
    const int dx;
    const int dy;

    // Hardware RelativePointerEvent will be split into ButtonEvent and PointerMove.
    // (in EventInputQueue::push_RelativePointerEventCallback.)
    //
    // ex_button is justPressed/justReleased PointingButton.
    // ex_isbuttondown indicates "pressed" or "released".
    const PointingButton ex_button;
    const bool ex_isbuttondown;

  private:
    Params_RelativePointerEventCallback(Buttons bt, int x, int y, PointingButton ex_btn, bool ex_isdown) : buttons(bt), dx(x), dy(y), ex_button(ex_btn), ex_isbuttondown(ex_isdown) {
      CommonData::increase_alloccount();
    }
  };
  class Params_ScrollWheelEventCallback {
    friend class EventOutputQueue;

  public:
    ~Params_ScrollWheelEventCallback(void) {
      CommonData::decrease_alloccount();
    }

    // Use auto_ptr instead allocating in kernel stack. (Reduce kernel stack usage.)
    DECLARE_AUTO_PTR(Params_ScrollWheelEventCallback);

    static Params_ScrollWheelEventCallback* alloc(short da1, short da2, short da3,
                                                  IOFixed fd1, IOFixed fd2, IOFixed fd3,
                                                  SInt32 pd1, SInt32 pd2, SInt32 pd3,
                                                  SInt32 op) {
      return new Params_ScrollWheelEventCallback(da1, da2, da3,
                                                 fd1, fd2, fd3,
                                                 pd1, pd2, pd3,
                                                 op);
    }

    static Params_ScrollWheelEventCallback* alloc(const Params_ScrollWheelEventCallback& p) {
      return new Params_ScrollWheelEventCallback(p.deltaAxis1,
                                                 p.deltaAxis2,
                                                 p.deltaAxis3,
                                                 p.fixedDelta1,
                                                 p.fixedDelta2,
                                                 p.fixedDelta3,
                                                 p.pointDelta1,
                                                 p.pointDelta2,
                                                 p.pointDelta3,
                                                 p.options);
    }

    static void log(bool isCaught,
                    short deltaAxis1,
                    short deltaAxis2,
                    short deltaAxis3,
                    IOFixed fixedDelta1,
                    IOFixed fixedDelta2,
                    IOFixed fixedDelta3,
                    SInt32 pointDelta1,
                    SInt32 pointDelta2,
                    SInt32 pointDelta3,
                    SInt32 options) {
      IOLOG_DEBUG_POINTING("ScrollWheelEventCallback [%7s]: deltaAxis(%d,%d,%d), fixedDelta(%d,%d,%d), pointDelta(%d,%d,%d), options: %d\n",
                           isCaught ? "caught" : "sending",
                           deltaAxis1, deltaAxis2, deltaAxis3,
                           fixedDelta1, fixedDelta2, fixedDelta3,
                           pointDelta1, pointDelta2, pointDelta3,
                           options);
    }

    const short deltaAxis1;
    const short deltaAxis2;
    const short deltaAxis3;
    const IOFixed fixedDelta1;
    const IOFixed fixedDelta2;
    const IOFixed fixedDelta3;
    const SInt32 pointDelta1;
    const SInt32 pointDelta2;
    const SInt32 pointDelta3;
    const SInt32 options;

  private:
    Params_ScrollWheelEventCallback(short da1, short da2, short da3,
                                    IOFixed fd1, IOFixed fd2, IOFixed fd3,
                                    SInt32 pd1, SInt32 pd2, SInt32 pd3,
                                    SInt32 op) :
      deltaAxis1(da1), deltaAxis2(da2), deltaAxis3(da3),
      fixedDelta1(fd1), fixedDelta2(fd2), fixedDelta3(fd3),
      pointDelta1(pd1), pointDelta2(pd2), pointDelta3(pd3),
      options(op) {
      CommonData::increase_alloccount();
    }
  };

  // This params is virtual parameter for EventOutputQueue.
  // Hardware does not send this event.
  class Params_Wait {
    friend class EventOutputQueue;

  public:
    ~Params_Wait(void) {
      CommonData::decrease_alloccount();
    }

    // Use auto_ptr instead allocating in kernel stack. (Reduce kernel stack usage.)
    DECLARE_AUTO_PTR(Params_Wait);

    static Params_Wait* alloc(int ms) {
      return new Params_Wait(ms);
    }
    static Params_Wait* alloc(const Params_Wait& p) {
      return new Params_Wait(p.milliseconds);
    }

    const int milliseconds;

  private:
    Params_Wait(int ms) : milliseconds(ms) {
      CommonData::increase_alloccount();
    }
  };
}

#endif
