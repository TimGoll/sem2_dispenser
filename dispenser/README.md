# Dispenser Software

## Needed Libraries

1. Adafruit SSD1306
2. Adafruit GFX
3. Adafruit RTClib

## Software

### Registering Button Callbacks

Button callbacks are a neat system to easily design the software to do actions on the press of a button. Since no memberfunctions of a class can be passed as an callable callback pointer, the function has to be static. The class reference is then passed as an argument.

```cpp
this->buttonHandler->addCallback(10, RISING, &callbackTest, this);
```

The above shown example registers a callback function `callbackTest` and also passes the parent scope with `this`. Therefore all functions and variables of the class can be used in the callback.

```cpp
static void Main::callbackTest(Main* self, uint8_t type) {
    // code here
}
```

This shows the structure of the callback function. The `self` parameter has to be the type of the class itself and can be used inthe same way as `this` is normally used. The type is specific to this button callback and defines the type of the button event (RISING, FALLING, HIGH).
