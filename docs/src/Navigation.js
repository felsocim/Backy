import React, { Component } from 'react';
import PropTypes from 'prop-types';
import 'semantic-ui-css/semantic.min.css';
import { Menu } from 'semantic-ui-react';
import { isSetOrDefault } from './common';
import { defaultLocale, translate } from "./strings";

class Navigation extends Component {
  constructor(props) {
    super(props);
    this.state = {
      scrolled: isSetOrDefault(this.props.scrolled, false),
      locale: isSetOrDefault(this.props.locale, defaultLocale)
    };
  }

  render() {
    return (
      <Menu stackable>
        <Menu.Item
          onClick={this.props.handler}
        >
          { translate('NAVIGATION_HOME') }
        </Menu.Item>
      </Menu>
    );
  }
}

Navigation.PropTypes = {
  scrolled: PropTypes.bool,
  locale: PropTypes.string
};

export default Navigation;