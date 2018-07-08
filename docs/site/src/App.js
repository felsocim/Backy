import React, { Component } from "react";
import Cookies from "universal-cookie"
import { Segment } from "semantic-ui-react";
import Navigation from "./Navigation";
import { About, Usage, Documentation, Download, Error404 } from "./Pages";
import { defaultLanguage, translate } from "./strings";
import "semantic-ui-css/semantic.min.css";
import "./Backy.css";

class App extends Component {
  constructor(props) {
    super(props);
    this.cookie = new Cookies();
    this.state = {
      page: "about",
      language: this.cookie.get("language") === null || this.cookie.get("language") === "undefined" ? defaultLanguage : this.cookie.get("language")
    };
    this.pageSelectionHandler = this.pageSelectionHandler.bind(this);
  }

  pageSelectionHandler(newPage, newLanguage) {
    this.setState({ page: newPage, language: newLanguage });
    this.cookie.set("language", newLanguage);
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
      <div className="Backy">
        <Navigation page={this.state.page} language={this.state.language} handleMenuItemClick={this.pageSelectionHandler}/>
        <div className="page">
          { display }
          <Segment className="copying"><span dangerouslySetInnerHTML={{ __html: translate("FOOTER", this.state.language) }}/></Segment>
        </div>
      </div>
    );
  }
}

export default App;
