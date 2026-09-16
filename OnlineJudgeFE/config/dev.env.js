let buildDate = require('moment')().format('YYYY-MM-DD')
let { VERSION } = require('./version')

console.log(`current version is ${VERSION} (built ${buildDate})`)

module.exports = {
  NODE_ENV: '"development"',
  VERSION: `"${VERSION}"`,
  BUILD_DATE: `"${buildDate}"`,
  USE_SENTRY: '0'
}
