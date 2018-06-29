import React, { Component } from 'react';
import logo from './logo.svg';
import './App.css';
import Navigation from './Navigation';

class App extends Component {
  testHandler() {
    alert("Giducová");
  }
  render() {
    return (
      <div className="App">
        <Navigation handler={this.testHandler.bind(this)}/>
        <header className="App-header">
          <img src={logo} className="App-logo" alt="logo" />
          <h1 className="App-title">Vitajte na stránke projektu Backy!</h1>
        </header>
        <p className="App-intro">
          To get started, edit <code>src/App.js</code> and save to reload.
        </p>
      </div>
    );
  }
}

export default App;
