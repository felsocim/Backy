import React, { Component } from "react";
import { Segment } from "semantic-ui-react";
import Navigation from "./Navigation";
import {About, Usage, Documentation, Download, Error404 } from "./Pages";
import { defaultLanguage, translate } from "./strings";
import "semantic-ui-css/semantic.min.css";
import "./App.css";

class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      page: "about",
      language: defaultLanguage
    };
    this.pageSelectionHandler = this.pageSelectionHandler.bind(this);
  }

  pageSelectionHandler(newPage, newLanguage) {
    this.setState({ page: newPage, language: newLanguage });
  }

  render() {
    let display = <Error404 language={this.state.language}/>;

    switch(this.state.page) {
      case "about":
        display = <About language={this.state.language}/>;
        break;
      case "usage":
        display = <Usage language={this.state.language}/>;
        break;
      case "documentation":
        display = <Documentation language={this.state.language}/>;
        break;
      case "get":
        display = <Download language={this.state.language}/>;
        break;
      default:
        /* By default, display error 404 */
        break;
    }

    return (
      <div className="App">
        <Navigation page={this.state.page} language={this.state.language} handleMenuItemClick={this.pageSelectionHandler}/>
        { display }
        <Segment className="copyright"><span dangerouslySetInnerHTML={{ __html: translate("FOOTER", this.state.language) }}/></Segment>
      </div>
    );
  }
}

export default App;
