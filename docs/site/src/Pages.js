import React, { Component } from "react";
import PropTypes from "prop-types";
import { Header } from "semantic-ui-react";
import { translate } from "./strings";
import gui from "./res/gui.png";

class About extends Component {
  render() {
    return(
      <div>
        <Header as="h1">{ translate("ABOUT_INTRODUCTION_HEADER", this.props.language) }</Header>
        <div dangerouslySetInnerHTML={{ __html: translate("ABOUT_INTRODUCTION_CONTENT", this.props.language) }}/>
        <Header as="h2">{ translate("ABOUT_AUTHOR_HEADER", this.props.language) }</Header>
        <div dangerouslySetInnerHTML={{ __html: translate("ABOUT_AUTHOR_CONTENT", this.props.language) }}/>
        <Header as="h2">{ translate("ABOUT_CONTACT_HEADER", this.props.language) }</Header>
        <div dangerouslySetInnerHTML={{ __html: translate("ABOUT_CONTACT_CONTENT", this.props.language) }}/>
        <Header as="h2">{ translate("ABOUT_LICENSE_HEADER", this.props.language) }</Header>
        <div dangerouslySetInnerHTML={{ __html: translate("ABOUT_LICENSE_CONTENT", this.props.language) }}/>
      </div>
    );
  }
}

About.propTypes = {
  language: PropTypes.string
};

class Usage extends Component {
  render() {
    return(
      <div>
        <Header as="h1">{ translate("USAGE_INTRODUCTION_HEADER", this.props.language) }</Header>
        <div dangerouslySetInnerHTML={{ __html: translate("USAGE_INTRODUCTION_CONTENT", this.props.language) }}/>
        <img src={ gui } alt="Backy" className="gui"/>
        <Header as="h2">{ translate("USAGE_CREATE_BACKUP_HEADER", this.props.language) }</Header>
        <div dangerouslySetInnerHTML={{ __html: translate("USAGE_CREATE_BACKUP_CONTENT", this.props.language) }}/>
        <Header as="h2">{ translate("USAGE_UPDATE_BACKUP_HEADER", this.props.language) }</Header>
        <div dangerouslySetInnerHTML={{ __html: translate("USAGE_UPDATE_BACKUP_CONTENT", this.props.language) }}/>
        <Header as="h2">{ translate("USAGE_ABORT_BACKUP_HEADER", this.props.language) }</Header>
        <div dangerouslySetInnerHTML={{ __html: translate("USAGE_ABORT_BACKUP_CONTENT", this.props.language) }}/>
        <Header as="h2">{ translate("USAGE_CHANGE_DISPLAY_LANGUAGE_HEADER", this.props.language) }</Header>
        <div dangerouslySetInnerHTML={{ __html: translate("USAGE_CHANGE_DISPLAY_LANGUAGE_CONTENT", this.props.language) }}/>
        <Header as="h2">{ translate("USAGE_MAXIMAL_COPY_BUFFER_SIZE_HEADER", this.props.language) }</Header>
        <div dangerouslySetInnerHTML={{ __html: translate("USAGE_MAXIMAL_COPY_BUFFER_SIZE_CONTENT", this.props.language) }}/>
      </div>
    );
  }
}

Usage.propTypes = {
  language: PropTypes.string
};

class Documentation extends Component {
  render() {
    return(
      <div>
        <Header as="h1">{ translate("DOCUMENTATION_HEADER", this.props.language) }</Header>
        <div dangerouslySetInnerHTML={{ __html: translate("DOCUMENTATION_CONTENT", this.props.language) }}/>
      </div>
    );
  }
}

Documentation.propTypes = {
  language: PropTypes.string
};

class Download extends Component {
  render() {
    return(
      <div>
        <Header as="h1">{ translate("GET_BACKY_HEADER", this.props.language) }</Header>
        <div dangerouslySetInnerHTML={{ __html: translate("GET_BACKY_CONTENT", this.props.language) }}/>
      </div>
    );
  }
}

Download.propTypes = {
  language: PropTypes.string
};

class Error404 extends Component {
  render() {
    return(
      <div>
        <Header as="h1">{ translate("ERROR_NOT_FOUND_HEADER", this.props.language) }</Header>
        <div dangerouslySetInnerHTML={{ __html: translate("ERROR_NOT_FOUND_CONTENT", this.props.language) }}/>
      </div>
    );
  }
}

Error404.propTypes = {
  language: PropTypes.string
};

export { About, Usage, Documentation, Download, Error404 };
