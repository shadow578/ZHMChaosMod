import { EntityParserFunction, EntityInfo } from "../types";
import { parse as parseCppt } from "./cppt";
import { parse as parseBp } from "./bp";
import { parse as parseBpPartial } from "./bp-partial";

const parsers: EntityParserFunction[] = [parseCppt, parseBp, parseBpPartial];

const typeOverrides: Record<string, string> = {
  ZEntityReference: "ZEntityRef",
  SEntityTemplateReference: "ZEntityRef",
};

/**
 * Parses entity data using the first compatible parser.
 * @param data Entity data, already parsed from JSON to js object.
 * @returns parsed data, or null if parsing failed.
 */
export function parse(data: object): EntityInfo | null {
  for (const parser of parsers) {
    const result = parser(data);
    if (result) {
      // apply type overrides
      for (const prop of result.properties) {
        if (prop.type && prop.type in typeOverrides) {
          prop.type = typeOverrides[prop.type];
        }
      }

      return result;
    }
  }

  return null;
}
