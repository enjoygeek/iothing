/*--------------------------------------------------------------------------*
* Simple JSON parser/generator for microcontrollers.
*---------------------------------------------------------------------------*
* 27-Jan-2016 ShaneG
*
* Initial implementation.
*--------------------------------------------------------------------------*/
#ifndef __JSON_H
#define __JSON_H

/** JSON data types
 */
typedef enum {
  JsonPrimitive = 0, // Boolean, null and numbers
  JsonObject,        // Objects (dictionary type)
  JsonArray,         // Arrays of values
  JsonString         // Quoted strings
  } JsonType;

/** Single JSON token
 */
typedef struct {
  JsonType type;  /* Token type */
  int      start; /* Token start position */
  int      end;   /* Token end position */
  int      size;  /* Number of child (nested) tokens */
  } JsonToken;

/** Parser for JSON content
 */
class JsonParser {
  public:
    /** Initialise the parser with space to keep tokens
     *
     * @param pTokens pointer to an array of JsonToken structures
     * @param tokens the maximum number of tokens that can be stored.
     */
    void begin(JsonToken *pTokens, int tokens);

    /** Parse JSON from a string buffer in memory
     *
     * @param cszJson pointer to a NUL terminated string containing the JSON
     *                to be parsed.
     *
     * @return the number of tokens discovered or a negative value if an error
     *         occurs.
     */
    int parse(const char *cszJson);
  };

// Builder state values
typedef enum {
  BuildBase,
  BuildObject,
  BuildArray
  } JsonBuilderState;

// Maximum nesting depth
#define MAX_DEPTH 8

/** Helper class to build a JSON string in memory
 */
class JsonBuilder {
  private:
    char            *m_szBuffer;
    int              m_index;
    int              m_size;
    JsonBuilderState m_state[MAX_DEPTH];
    int              m_depth;

  protected:
    /** Initialise the state
     */
    void init();

  public:
    /** Default constructor
     */
    JsonBuilder();

    /** Initialise the builder
     *
     * The builder always creates an 'object' and adds a '{' character to the
     * start of the buffer. Additional values are then added with the
     * appropriate 'addXXX()' methods.
     *
     * @param szBuffer pointer to a buffer to hold the generated JSON
     * @param size the number of characters the buffer can hold.
     */
    void begin(char *szBuffer, int size);

    /** Add a string value to the current object
     *
     * @param cszName the name of the new value
     * @param cszValue the value to associated with the name.
     *
     * @return true if the value was added, false if the buffer is full.
     */
    bool add(const char *cszName, const char *cszValue);

    /** Add a boolean value to the current object
     *
     * @param cszName the name of the new value
     * @param value the value to associated with the name.
     *
     * @return true if the value was added, false if the buffer is full.
     */
    bool add(const char *cszName, bool value);

    /** Add a integer value to the current object
     *
     * @param cszName the name of the new value
     * @param value the value to associated with the name.
     *
     * @return true if the value was added, false if the buffer is full.
     */
    bool add(const char *cszName, int value);

    /** Add a floating point value to the current object
     *
     * @param cszName the name of the new value
     * @param value the value to associated with the name.
     *
     * @return true if the value was added, false if the buffer is full.
     */
    bool add(const char *cszName, double value);

    /** Add a new child object to the current object
     *
     * Adds a new field to the current object that will contain a child
     * object. Future calls to addXXX(name, value) will add fields to the
     * child object.
     *
     * @param cszName the name of the new field.
     *
     * @return true on success, false if the buffer is full or the operation
     *         is not available in the current state.
     */
    bool beginObject(const char *cszName);

    /** End the current child object
     *
     * @return true on success, false if the buffer is full or the operation
     *         is not available in the current state.
     */
    bool endObject();

    /** Add a new child array to the current object
     *
     * @param cszName the name of the new field.
     *
     * @return true on success, false if the buffer is full or the operation
     *         is not available in the current state.
     */
    bool beginArray(const char *cszName);

    /** End the current child array
     *
     * @return true on success, false if the buffer is full or the operation
     *         is not available in the current state.
     */
    bool endArray();

    /** Add a new string to the current array
     *
     * @param cszValue the value to add to the array
     *
     * @return true on success, false if the buffer is full or the builder is
     *         not currently building an array.
     */
    bool add(const char *cszValue);

    /** Add a new boolean value to the current array
     *
     * @param value the value to add to the array
     *
     * @return true on success, false if the buffer is full or the builder is
     *         not currently building an array.
     */
    bool add(bool value);

    /** Add a new integer value to the current array
     *
     * @param value the value to add to the array
     *
     * @return true on success, false if the buffer is full or the builder is
     *         not currently building an array.
     */
    bool add(int value);

    /** Add a new floating point value to the current array
     *
     * @param value the value to add to the array
     *
     * @return true on success, false if the buffer is full or the builder is
     *         not currently building an array.
     */
    bool add(double value);

    /** Finish building.
     *
     * This closes all current open objects and arrays and terminates the
     * string in the output buffer.
     *
     * @return the number of characters (excluding the NUL terminator) in the
     *         buffer or 0 if the buffer is full.
     */
    int end();
  };

#endif /* __JSON_H */