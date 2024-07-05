import React from 'react';
import { KeycloakProvider, useKeycloak } from './components/KeycloakProvider';

const HomePage = () => {
  const { keycloak, authenticated, keycloakInitialized } = useKeycloak();

  if (!keycloakInitialized) {
    return <div>Loading...</div>;
  }

  return (
    <div>
      {authenticated ? (
        <div>
          <p>Login successful!</p>
          <p>Welcome, {keycloak.tokenParsed.preferred_username}</p>
        </div>
      ) : (
        <div>Unable to authenticate!</div>
      )}
    </div>
  );
};

const App = () => (
  <KeycloakProvider>
    <HomePage />
  </KeycloakProvider>
);

export default App;
