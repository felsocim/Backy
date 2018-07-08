import React, { Component } from "react";
import PropTypes from "prop-types";
import { Menu, Dropdown } from "semantic-ui-react";
import { translate } from "./strings";
import logo from "./res/backy_small.png";

class Navigation extends Component {
  onMenuItemClick = (e, { name }) => {
    this.props.handleMenuItemClick(name, this.props.language);
  };

  onLanguageClick = (e, { name }) => {
    this.props.handleMenuItemClick(this.props.page, name);
  };

  render() {
    return (
      <Menu inverted stackable>
        <Menu.Item>
          <img src={ logo } alt="Backy"/><span className="logo">Backy</span>
        </Menu.Item>
        <Menu.Item
          name="about"
          active={ this.props.page === "about" }
          onClick={ this.onMenuItemClick }
        >
          { translate("NAVIGATION_ABOUT", this.props.language) }
        </Menu.Item>
        <Menu.Item
          name="usage"
          active={ this.props.page === "usage" }
          onClick={ this.onMenuItemClick }
        >
          { translate("NAVIGATION_USAGE", this.props.language) }
        </Menu.Item>
        <Menu.Item
          name="documentation"
          active={ this.props.page === "documentation" }
          onClick={ this.onMenuItemClick }
        >
          { translate("NAVIGATION_DOCUMENTATION", this.props.language) }
        </Menu.Item>
        <Menu.Item
          name="get"
          active={ this.props.page === "get" }
          onClick={ this.onMenuItemClick }
        >
          { translate("NAVIGATION_GET", this.props.language) }
        </Menu.Item>

        <Menu.Menu position="right">
          <Dropdown
            item
            text={ translate("NAVIGATION_LANGUAGE") }
          >
            <Dropdown.Menu>
              <Dropdown.Item
                name="en"
                active={ this.props.language === "en" }
                onClick={ this.onLanguageClick }
              >
                English
              </Dropdown.Item>
              <Dropdown.Item
                name="fr"
                active={ this.props.language === "fr" }
                onClick={ this.onLanguageClick }
              >
                français
              </Dropdown.Item>
              <Dropdown.Item
                name="sk"
                active={ this.props.language === "sk" }
                onClick={ this.onLanguageClick }
              >
                slovenčina
              </Dropdown.Item>
            </Dropdown.Menu>
          </Dropdown>
        </Menu.Menu>
      </Menu>
    );
  }
}

Navigation.propTypes = {
  page: PropTypes.string,
  language: PropTypes.string,
  handleMenuItemClick: PropTypes.func
};

export default Navigation;
