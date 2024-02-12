package com.vpavlov.pockerclient.ui.i18n;

import com.vpavlov.pockerclient.App;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Locale;
import java.util.Properties;

public class I18n extends Properties {

    private static final Logger LOGGER = LogManager.getLogger(I18n.class);

    private static final String I18N_FOLDER = "config/i18n/";

    private static final String I18N_FILE_PREFIX = "translations_sss";

    private static final String I18N_FILE_EXT = ".properties";

    public static I18n load(Locale locale) {
        I18n prop = new I18n();
        String filePath = I18N_FOLDER + I18N_FILE_PREFIX + locale.toString() + I18N_FILE_EXT;
        URL resUrl = App.class.getResource(filePath);
        if (resUrl != null) {
            try {
                prop.load(new FileInputStream(resUrl.getPath()));
            } catch (IOException e) {
                LOGGER.error(String.format("An error occurred during '%s' file load.", resUrl.getPath()));
            }
        } else {
            LOGGER.warn(String.format("Localization file '%s' was not found", filePath));
        }
        return prop;
    }

    public String getI18n(String key){
        String prop = getProperty(key);
        return prop == null ? key : prop;
    }


}
