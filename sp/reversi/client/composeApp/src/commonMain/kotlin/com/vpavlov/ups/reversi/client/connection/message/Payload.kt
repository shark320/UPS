package com.vpavlov.ups.reversi.client.connection.message

import com.vpavlov.ups.reversi.client.connection.MSG_HEADER_LENGTH
import java.util.regex.Matcher
import java.util.regex.Pattern


private val ALLOWED_TYPES = setOf(
    String::class,
    Int::class,
    Boolean::class,
    List::class
)

private const val LIST_START = '['

private const val LIST_END = ']'

private const val SEPARATOR = ';'

private var KEY_SEPARATOR = "="

private var LIST_SEPARATOR = ","


val LIST_PATTERN: Pattern = Pattern.compile("^\\[((\"[^\"]*(?:\",\"[^\"]*)*\")|\\d+(?:,\\d+)*)]$")

val LIST_INT_PATTERN: Pattern = Pattern.compile("^\\[(\\d+(?:,\\d+)*)]$")

val LIST_STRING_PATTERN: Pattern = Pattern.compile("^\\[(\"[^\"]*(?:\",\"[^\"]*)*\")]$")

val STRING_PATTERN: Pattern = Pattern.compile("^\"([^\"]*)\"$")

val INT_PATTERN: Pattern = Pattern.compile("^\\d+$")

val BOOL_PATTERN: Pattern = Pattern.compile("^(true|false)$")

val NULL_PATTERN: Pattern = Pattern.compile("^(null)$")



data class Payload(val data: MutableMap<String, Any?> = mutableMapOf()) {

    @Synchronized
    fun setValue(key: String, value: Any?) {
        require(key.isNotBlank()) {
            "key could not be blank."
        }
        if (value == null){
            data[key] = null
            return;
        }
        require(value::class in ALLOWED_TYPES) {
            "value should be of type: $ALLOWED_TYPES."
        }
        require(value is List<*> && value.all { it in ALLOWED_TYPES }) {
            "value should be of type: $ALLOWED_TYPES."
        }
        data[key] = value
    }

    @Synchronized
    fun getValue(key: String): Any? = data[key]

    @Synchronized
    fun construct(): String {
        if (data.isEmpty()) {
            return ""
        }
        val sb = StringBuilder()
        data.forEach { (key, value) ->
            sb.append(key).append(KEY_SEPARATOR)
            sb.append(map(value)).append(SEPARATOR)
        }
        return sb.toString()
    }

    private fun map(value: Any?): String?{
        if (value == null){
            return "null"
        }
        return when(value){
            is String -> mapString(value)
            is Int -> mapInt(value)
            is Boolean -> mapBoolean(value)
            is List<*> -> mapList(value)
            else -> null
        }
    }

    private fun mapString(value: String): String = "\"$value\""

    private fun mapStringsList(value: List<String>): String{
        val sb = java.lang.StringBuilder()
        for (string in value) {
            sb.append(mapString(string)).append(',')
        }
        sb.deleteCharAt(sb.length - 1)
        return sb.toString()
    }

    private fun mapInt (value: Int) = value.toString()

    private fun mapIntList(value: List<Int>): String {
        val sb = java.lang.StringBuilder()
        for (integer in value) {
            sb.append(mapInt(integer)).append(',')
        }

        //delete last comma
        sb.deleteCharAt(sb.length - 1)
        return sb.toString()
    }

    private fun mapList(value: List<*>): String {
        if (value.isEmpty()){
            return "$LIST_START$LIST_END"
        }
        val result = when(value[0]){
            is Int -> mapIntList(value as List<Int>)
            is String -> mapStringsList(value as List<String>)
            else -> ""
        }
        return "$LIST_START$result$LIST_END"
    }

    private fun mapBoolean(value: Boolean) = value.toString()

    companion object{
        fun parse(message: String): Payload{
            if (message.length <= MSG_HEADER_LENGTH){
                return Payload()
            }
            val payloadStr = message.substring(MSG_HEADER_LENGTH)
            return parseHelper(payloadStr)
        }

        private fun parseHelper(payloadStr: String): Payload {
            val payload = Payload()
            if (payloadStr.isBlank()){
                return payload
            }
            val payloadTokens = payloadStr.split(SEPARATOR)
            payloadTokens.forEach { token->
                parseToken(token, payload)
            }
            return payload
        }

        private fun parseToken(token: String, payload: Payload){
            val keyValueTokens = token.split(KEY_SEPARATOR)
            require (keyValueTokens.size == 2){
                "Unable to parse token: $token"
            }
            val key = keyValueTokens[0]
            require(key.isNotBlank()){
                "key could not be blank."
            }
            val value = keyValueTokens[1]
            payload.setValue(key,parseValue(value))
        }

        private fun parseValue(value: String): Any? {
            val trimmed = value.trim()
            val nullMatcher: Matcher = NULL_PATTERN.matcher(trimmed)
            if (nullMatcher.matches()) {
                return null
            }
            val listMatcher: Matcher = LIST_PATTERN.matcher(trimmed)
            if (listMatcher.matches()) {
                return parsePayloadList(trimmed)
            }
            return parsePayloadValue(trimmed)
        }

        private fun parsePayloadValue(value: String): Any {
            if (STRING_PATTERN.matcher(value).matches()) {
                return parsePayloadString(value)
            }
            if (INT_PATTERN.matcher(value).matches()) {
                return parsePayloadInt(value)
            }
            if ( BOOL_PATTERN.matcher(value).matches()) {
                return value.toBoolean()
            }
            throw IllegalArgumentException("Invalid value: $value")
        }

        private fun parsePayloadString(value: String) = value.substring(1,value.length-1)

        private fun parsePayloadInt(value: String) = Integer.parseInt(value)

        private fun parsePayloadList(value: String): List<Any> {
            if (LIST_STRING_PATTERN.matcher(value).matches()) {
                return parseStringList(value)
            }
            if (LIST_INT_PATTERN.matcher(value).matches()) {
                return parseIntList(value)
            }
            throw java.lang.IllegalArgumentException("Invalid list type: $value")
        }

        private fun parseStringList(value: String): List<String> {
            val trimmed = value.substring(1, value.length - 1)
            val result = mutableListOf<String>()
            val tokens = trimmed.split(LIST_SEPARATOR)
            for (token in tokens) {
                result.add(parsePayloadString(token))
            }
            return result
        }

        private fun parseIntList(value: String): List<Int> {
            val trimmed = value.substring(1, value.length - 1)
            val result = mutableListOf<Int>()
            val tokens = trimmed.split(LIST_SEPARATOR)
            for (token in tokens) {
                result.add(parsePayloadInt(token))
            }
            return result
        }
    }

}