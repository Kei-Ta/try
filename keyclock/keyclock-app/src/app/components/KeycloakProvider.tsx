"use client"
import React, { createContext, useContext, useState, useEffect } from 'react';
import keycloak from '../../lib/Keycloak.ts';

const KeycloakContext = createContext({
    keycloak: null,
    authenticated: false,
    keycloakInitialized: false
});

export const KeycloakProvider = ({ children }) => {
    const [authenticated, setAuthenticated] = useState(false);
    const [keycloakInitialized, setKeycloakInitialized] = useState(false);

    useEffect(() => {
        keycloak.init({ onLoad: 'login-required' }).then(authenticated => {
            setAuthenticated(authenticated);
            setKeycloakInitialized(true);
        }).catch(error => {
            console.error('Keycloak initialization failed:', error);
        });
    }, []);

    return (
        <KeycloakContext.Provider value={{ keycloak, authenticated, keycloakInitialized }}>
            {children}
        </KeycloakContext.Provider>
    );
};

export const useKeycloak = () => useContext(KeycloakContext);
