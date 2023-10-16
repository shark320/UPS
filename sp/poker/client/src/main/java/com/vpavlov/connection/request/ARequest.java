package com.vpavlov.connection.request;

import com.vpavlov.connection.payload.APayload;
import com.vpavlov.connection.type.Subtype;
import com.vpavlov.connection.type.Type;

public abstract class ARequest {

    private Type type;

    private Subtype subtype;

    private APayload payload;
}
