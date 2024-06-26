KEY = {
    UNKNOWN = 0,
    SPACE = 32,  -- GLFW_KEY_SPACE
    APOSTROPHE = 39,  -- GLFW_KEY_APOSTROPHE
    COMMA = 44,  -- GLFW_KEY_COMMA
    MINUS = 45,  -- GLFW_KEY_MINUS
    PERIOD = 46,  -- GLFW_KEY_PERIOD
    SLASH = 47,  -- GLFW_KEY_SLASH
    ZERO = 48,  -- GLFW_KEY_0
    ONE = 49,  -- GLFW_KEY_1
    TWO = 50,  -- GLFW_KEY_2
    THREE = 51,  -- GLFW_KEY_3
    FOUR = 52,  -- GLFW_KEY_4
    FIVE = 53,  -- GLFW_KEY_5
    SIX = 54,  -- GLFW_KEY_6
    SEVEN = 55,  -- GLFW_KEY_7
    EIGHT = 56,  -- GLFW_KEY_8
    NINE = 57,  -- GLFW_KEY_9
    SEMICOLON = 59,  -- GLFW_KEY_SEMICOLON
    EQUAL = 61,  -- GLFW_KEY_EQUAL
    A = 65,  -- GLFW_KEY_A
    B = 66,  -- GLFW_KEY_B
    C = 67,  -- GLFW_KEY_C
    D = 68,  -- GLFW_KEY_D
    E = 69,  -- GLFW_KEY_E
    F = 70,  -- GLFW_KEY_F
    G = 71,  -- GLFW_KEY_G
    H = 72,  -- GLFW_KEY_H
    I = 73,  -- GLFW_KEY_I
    J = 74,  -- GLFW_KEY_J
    K = 75,  -- GLFW_KEY_K
    L = 76,  -- GLFW_KEY_L
    M = 77,  -- GLFW_KEY_M
    N = 78,  -- GLFW_KEY_N
    O = 79,  -- GLFW_KEY_O
    P = 80,  -- GLFW_KEY_P
    Q = 81,  -- GLFW_KEY_Q
    R = 82,  -- GLFW_KEY_R
    S = 83,  -- GLFW_KEY_S
    T = 84,  -- GLFW_KEY_T
    U = 85,  -- GLFW_KEY_U
    V = 86,  -- GLFW_KEY_V
    W = 87,  -- GLFW_KEY_W
    X = 88,  -- GLFW_KEY_X
    Y = 89,  -- GLFW_KEY_Y
    Z = 90,  -- GLFW_KEY_Z
    LEFT_BRACKET = 91,  -- GLFW_KEY_LEFT_BRACKET
    BACKSLASH = 92,  -- GLFW_KEY_BACKSLASH
    RIGHT_BRACKET = 93,  -- GLFW_KEY_RIGHT_BRACKET
    GRAVE_ACCENT = 96,  -- GLFW_KEY_GRAVE_ACCENT
    WORLD_1 = 161,  -- GLFW_KEY_WORLD_1
    WORLD_2 = 162,  -- GLFW_KEY_WORLD_2
    ESCAPE = 256,  -- GLFW_KEY_ESCAPE
    ENTER = 257,  -- GLFW_KEY_ENTER
    TAB = 258,  -- GLFW_KEY_TAB
    BACKSPACE = 259,  -- GLFW_KEY_BACKSPACE
    INSERT = 260,  -- GLFW_KEY_INSERT
    DELETE = 261,  -- GLFW_KEY_DELETE
    RIGHT = 262,  -- GLFW_KEY_RIGHT
    LEFT = 263,  -- GLFW_KEY_LEFT
    DOWN = 264,  -- GLFW_KEY_DOWN
    UP = 265,  -- GLFW_KEY_UP
    PAGE_UP = 266,  -- GLFW_KEY_PAGE_UP
    PAGE_DOWN = 267,  -- GLFW_KEY_PAGE_DOWN
    HOME = 268,  -- GLFW_KEY_HOME
    END = 269,  -- GLFW_KEY_END
    CAPS_LOCK = 280,  -- GLFW_KEY_CAPS_LOCK
    SCROLL_LOCK = 281,  -- GLFW_KEY_SCROLL_LOCK
    NUM_LOCK = 282,  -- GLFW_KEY_NUM_LOCK
    PRINT_SCREEN = 283,  -- GLFW_KEY_PRINT_SCREEN
    PAUSE = 284,  -- GLFW_KEY_PAUSE
    F1 = 290,  -- GLFW_KEY_F1
    F2 = 291,  -- GLFW_KEY_F2
    F3 = 292,  -- GLFW_KEY_F3
    F4 = 293,  -- GLFW_KEY_F4
    F5 = 294,  -- GLFW_KEY_F5
    F6 = 295,  -- GLFW_KEY_F6
    F7 = 296,  -- GLFW_KEY_F7
    F8 = 297,  -- GLFW_KEY_F8
    F9 = 298,  -- GLFW_KEY_F9
    F10 = 299,  -- GLFW_KEY_F10
    F11 = 300,  -- GLFW_KEY_F11
    F12 = 301,  -- GLFW_KEY_F12
    F13 = 302,  -- GLFW_KEY_F13
    F14 = 303,  -- GLFW_KEY_F14
    F15 = 304,  -- GLFW_KEY_F15
    F16 = 305,  -- GLFW_KEY_F16
    F17 = 306,  -- GLFW_KEY_F17
    F18 = 307,  -- GLFW_KEY_F18
    F19 = 308,  -- GLFW_KEY_F19
    F20 = 309,  -- GLFW_KEY_F20
    F21 = 310,  -- GLFW_KEY_F21
    F22 = 311,  -- GLFW_KEY_F22
    F23 = 312,  -- GLFW_KEY_F23
    F24 = 313,  -- GLFW_KEY_F24
    F25 = 314,  -- GLFW_KEY_F25
    KP_0 = 320,  -- GLFW_KEY_KP_0
    KP_1 = 321,  -- GLFW_KEY_KP_1
    KP_2 = 322,  -- GLFW_KEY_KP_2
    KP_3 = 323,  -- GLFW_KEY_KP_3
    KP_4 = 324,  -- GLFW_KEY_KP_4
    KP_5 = 325,  -- GLFW_KEY_KP_5
    KP_6 = 326,  -- GLFW_KEY_KP_6
    KP_7 = 327,  -- GLFW_KEY_KP_7
    KP_8 = 328,  -- GLFW_KEY_KP_8
    KP_9 = 329,  -- GLFW_KEY_KP_9
    KP_DECIMAL = 330,  -- GLFW_KEY_KP_DECIMAL
    KP_DIVIDE = 331,  -- GLFW_KEY_KP_DIVIDE
    KP_MULTIPLY = 332,  -- GLFW_KEY_KP_MULTIPLY
    KP_SUBTRACT = 333,  -- GLFW_KEY_KP_SUBTRACT
    KP_ADD = 334,  -- GLFW_KEY_KP_ADD
    KP_ENTER = 335,  -- GLFW_KEY_KP_ENTER
    KP_EQUAL = 336,  -- GLFW_KEY_KP_EQUAL
    LEFT_SHIFT = 340,  -- GLFW_KEY_LEFT_SHIFT
    LEFT_CONTROL = 341,  -- GLFW_KEY_LEFT_CONTROL
    LEFT_ALT = 342,  -- GLFW_KEY_LEFT_ALT
    LEFT_SUPER = 343,  -- GLFW_KEY_LEFT_SUPER
    RIGHT_SHIFT = 344,  -- GLFW_KEY_RIGHT_SHIFT
    RIGHT_CONTROL = 345,  -- GLFW_KEY_RIGHT_CONTROL
    RIGHT_ALT = 346,  -- GLFW_KEY_RIGHT_ALT
    RIGHT_SUPER = 347,  -- GLFW_KEY_RIGHT_SUPER
    MENU = 348  -- GLFW_KEY_MENU
}

MOUSE = {
    BUTTON0 = 0,   -- GLFW_MOUSE_BUTTON_1
    BUTTON2 = 1,   -- GLFW_MOUSE_BUTTON_2
    BUTTON3 = 2,   -- GLFW_MOUSE_BUTTON_3
    BUTTON4 = 3,   -- GLFW_MOUSE_BUTTON_4
    BUTTON5 = 4,   -- GLFW_MOUSE_BUTTON_5
    BUTTON6 = 5,   -- GLFW_MOUSE_BUTTON_6
    BUTTON7 = 6,   -- GLFW_MOUSE_BUTTON_7
    LAST = 7, -- GLFW_MOUSE_BUTTON_LAST
    LEFT = 0, -- GLFW_MOUSE_BUTTON_LEFT
    RIGHT = 1, -- GLFW_MOUSE_BUTTON_RIGHT
    MIDDLE = 2 -- GLFW_MOUSE_BUTTON_MIDDLE
}
